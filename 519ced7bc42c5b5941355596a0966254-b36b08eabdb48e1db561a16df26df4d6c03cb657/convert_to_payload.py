from ctypes import BigEndianStructure
from PIL import Image, ImageEnhance
from paho.mqtt import client as mqtt_client
import numpy
import time as time

image_name = './carlee.png'

client_id = 'access-card-pi-client-3'

broker = 'broker.hivemq.com'
port = 1883

username = 'altrMqttUser'
password = 'zmCBgsFcKG'

topic = "faisal"

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

    # send clientConnect message with client_id
    client.publish(topic, "Client Connected")
    return client


def publish(client, payload):
    while True:
        result = client.publish(topic, payload)
        # result: [0, 1]
        status = result[0]
        if status == 0:
            print(f"Message was sent to the topic {topic}")
        else:
            print(f"Failed to send message to topic {topic}")
        time.sleep(5)


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

    image.show()

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
        # bigString = bigString + "\n"
    return bigString


# with open('faisal.txt', 'w') as file:
# 	file.write(payload)


def run():
    payload = make_payload(image_name)
    client = connect_mqtt()
    client.loop_start()
    publish(client, payload)


if __name__ == '__main__':
    run()
