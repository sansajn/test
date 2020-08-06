# MQTT publisher sample
import paho.mqtt.client as mqtt

client = mqtt.Client()
client.connect('localhost', 1883, 60)
client.publish('/test', 'hello from python!')
client.disconnect()
print('done!')
