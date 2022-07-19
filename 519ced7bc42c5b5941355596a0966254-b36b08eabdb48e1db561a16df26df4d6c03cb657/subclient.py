from paho.mqtt import client as mqtt_client

broker = 'broker.hivemq.com'
port = 1883

username = 'altrMqttUser'
password = 'zmCBgsFcKG'

topic = "charlie"


def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client('subscriber')
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)

    # send clientConnect message with client_id
    client.publish(topic, "Client Connected")
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        if msg.topic == 'charlie':
            print("Update badge request...")
            # json_message = json.loads(msg.payload.decode())
            # write image to make easy to debug
            # image_file = 'writeimage.jpg'
            # with open(image_file, "wb") as f:
            # try:
            # 	f.write(
            # 		base64.b64decode(json_message['badge'])
            # 	)
            # except KeyError:
            # 	print("Missing image key")
            # 	return
            # write_ink(image_file)

        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

    client.subscribe(topic)
    client.on_message = on_message


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_start()
    # publish(client)


if __name__ == '__main__':
    run()
