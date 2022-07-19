import base64
import json
import random
import time
import sys

from PIL import Image
from inky.inky_uc8159 import Inky
from paho.mqtt import client as mqtt_client

# this is the device_id which will show up in the portal.
client_id = 'access-card-pi-client-3'

#broker = 'portal.alt-r.ai'
#broker = '192.168.1.168'
# broker = 'localhost'
broker = '18.144.37.188'
port = 1883

username = 'altrMqttUser'
password = 'zmCBgsFcKG'


topic = [("hello", 1), (client_id, 1), ("update_badge", 1), ]


def connect_mqtt():
	def on_connect(client, userdata, flags, rc):
		if rc == 0:
			print("Connected to MQTT Broker!")
		else:
			print("Failed to connect, return code %d\n", rc)

	client = mqtt_client.Client(client_id)
	client.username_pw_set(username, password)
	client.on_connect = on_connect
	client.connect(broker, port)

	# send clientConnect message with client_id
	result = client.publish("clientConnect",
							json.dumps({"client_id": client_id}))
	return client


def publish(client):
	msg_count = 0
	while True:
		time.sleep(1)
		continue
	msg = f"messages: {msg_count}"
	# result = client.publish(topic, msg)
	result = client.publish("clientConnect", '{"client_id": client_id}')
	# result: [0, 1]
	status = result[0]
	if status == 0:
		print(f"Send `{msg}` to topic `{topic}`")
	else:
		print(f"Failed to send message to topic {topic}")
	msg_count += 1


def subscribe(client: mqtt_client):
	def on_message(client, userdata, msg):
		if msg.topic == 'update_badge':
			print("Update badge request...")
			json_message = json.loads(msg.payload.decode())
			# write image to make easy to debug
			image_file = 'writeimage.jpg'
			with open(image_file, "wb") as f:
				try:
					f.write(
						base64.b64decode(json_message['badge'])
					)
				except KeyError:
					print("Missing image key");
					return
				write_ink(image_file)

		print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

	client.subscribe(topic)
	client.on_message = on_message


def write_ink(image_file):
	inky = Inky()
	saturation = 0.5

	# Here we rotate the image 90 deg to write to display
	image = Image.open(image_file)
	image = image.rotate(90, expand=True);
	image = image.convert("RGB")
	image.save(image_file);
	image = Image.open(image_file)

	inky.set_image(image, saturation=saturation)
	try:
		inky.show()
	except Exception as e:
		# XXX inky.show rasies a runtime error improve handing
		# of this later.. https://github.com/pimoroni/inky/issues/103
		pass


def run():
	client = connect_mqtt()
	subscribe(client)
	client.loop_start()
	publish(client)


if __name__ == '__main__':
	run()