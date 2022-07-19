from PIL import Image, ImageEnhance
from paho.mqtt import client as mqtt_client
import numpy
import time as time

chunk_size = 5000

# width,height = 0
image_name = './carlee.png'

client_id = 'access-card-pi-client-3'

broker = 'broker.hivemq.com'
port = 1883

username = 'altrMqttUser'
password = 'zmCBgsFcKG'

topic_chunks = "faisal"
topic_chunks_size = "chunks-size"
topic_array_size = "array-size"

DESATURATED_PALETTE = [
    [0, 0, 0],
    [255, 255, 255],
    [0, 255, 0],
    [0, 0, 255],
    [255, 0, 0],
    [255, 255, 0],
    [255, 140, 0],
    [255, 255, 255]
]

SATURATED_PALETTE = [
    [57, 48, 57],
    [255, 255, 255],
    [58, 91, 70],
    [61, 59, 94],
    [156, 72, 75],
    [208, 190, 71],
    [177, 106, 73],
    [255, 255, 255]
]


def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def publish(client, payloadlist, NUMCHUNKS, ARRAYSIZE):

    print("The number of chunks generated are:",NUMCHUNKS)
    print("The array size is:",ARRAYSIZE)

    result = client.publish(topic_array_size, ARRAYSIZE)
    if result[0] == 0:
        print(f"The array size {ARRAYSIZE} was sent to the topic.")
    else:
        print(f"Failed to send the array size {ARRAYSIZE} to the topic.")

    iter = 1
    # Using for loop

    for i in payloadlist:
        result = client.publish(topic_chunks, str(i))
        if result[0] == 0:
            print(f"Chunk no. {iter} was sent to the topic.")
        else:
            print(f"Failed to send chunk no. {iter} to the topic.")
        iter = iter + 1
        time.sleep(1.5)
    # contentLength = len(payload)
    # cIndex = 0
    # iter = 1
    # while (cIndex < contentLength):
    #     chunk = payload[cIndex:cIndex+chunk_size]
    #     cIndex = cIndex + chunk_size
    #     result = client.publish(topic, chunk)
    #     status = result[0]
    #     if status == 0:
    #         print(f"Chunk no. {iter} was sent to the topic.")
    #     else:
    #         print(f"Failed to send chunk no. {iter} to the topic.")
    #     iter = iter + 1
    #     time.sleep(1.5)
    # time.sleep(3)
    # client.publish("finished", str(134400)) #width*height/2


def _palette_blend(saturation, dtype='uint8'):
    saturation = float(saturation)
    palette = []
    for i in range(7):
        rs, gs, bs = [c * saturation for c in SATURATED_PALETTE[i]]
        rd, gd, bd = [c * (1.0 - saturation) for c in DESATURATED_PALETTE[i]]
        if dtype == 'uint8':
            palette += [int(rs + rd), int(gs + gd), int(bs + bd)]
        if dtype == 'uint24':
            palette += [(int(rs + rd) << 16) |
                        (int(gs + gd) << 8) | int(bs + bd)]
    if dtype == 'uint8':
        palette += [255, 255, 255]
    if dtype == 'uint24':
        palette += [0xffffff]

    return palette


def make_payload(image_name):
    bigString = ""
    image = Image.open(image_name)
    image = image.convert('RGB')

    width, height = image.size
    print("width = ", width, " height = ", height)

    # image.show()

    saturation = 0.5
    palette = _palette_blend(saturation)
    palette_image = Image.new("P", (1, 1))
    palette_image.putpalette(palette + [0, 0, 0] * 248)

    converted_image = image.im.convert("P", True, palette_image.im)

    # the reshape method will allow us to access the buffer as buf[y][x]
    buf = numpy.array(converted_image, dtype=numpy.uint8).reshape(
        (height, width))

    for y in range(height):
        for x in range(int(width/2)):
            pix_left_val = int(buf[y][x*2])
            pix_right_val = int(buf[y][x*2+1])
            combined = pix_left_val << 4 | pix_right_val
            bigString = bigString + hex(combined)
            if x < int(width/2):
                bigString = bigString + ","
    return bigString


def make_payload_image_chunks(image_name):
    bigString = ""
    image = Image.open(image_name)
    image = image.convert('RGB')

    width, height = image.size

    saturation = 0.5
    palette = _palette_blend(saturation)
    palette_image = Image.new("P", (1, 1))
    palette_image.putpalette(palette + [0, 0, 0] * 248)

    converted_image = image.im.convert("P", True, palette_image.im)

    # the reshape method will allow us to access the buffer as buf[y][x]
    buf = numpy.array(converted_image, dtype=numpy.uint8).reshape(
        (height, width))

    CHUNKSIZE = 1024
    NUMCHUNKS = int(height*width/(2*CHUNKSIZE))
    ARRAYSIZE = height*width/2
    IMGMSG = "0x00"
    chunkmsg = IMGMSG + "," + hex(CHUNKSIZE) + "," + hex(NUMCHUNKS)
    # 0x00 shows that an image is incoming
    # 0x400 is the chunksize
    # 0x80 is the number of chunks

    IMGCHUNKS = []
    i = 0
    for y in range(height):
        for x in range(int(width/2)):
            pix_left_val = int(buf[y][x*2])
            pix_right_val = int(buf[y][x*2+1])
            combined = pix_left_val << 4 | pix_right_val
            bigString = bigString + hex(combined)
            if x < int(width/2):
                bigString = bigString + ","
            i += 1
            if(i % CHUNKSIZE == 0):
                IMGCHUNKS.append(bigString)
                bigString = ""
        bigString = bigString + "\n"

    return NUMCHUNKS, IMGCHUNKS, ARRAYSIZE

def run():
    NUMCHUNKS, chunklist, ARRAYSIZE = make_payload_image_chunks(image_name)
    client = connect_mqtt()
    client.loop_start()
    # input("Please press a key to proceed!")
    publish(client, chunklist, NUMCHUNKS, ARRAYSIZE)


if __name__ == '__main__':
    run()
