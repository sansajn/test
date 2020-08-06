# MQTT subscriber sample
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
	print('connected with result code %s' % str(rc))
	client.subscribe('/test')

def on_message(client, userdata, msg):
	print('received: %s' % msg.payload.decode())

client = mqtt.Client()
client.connect('localhost', 1883, 60)
client.on_connect = on_connect
client.on_message = on_message
client.loop_forever()
