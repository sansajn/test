# Weather update client
# Connects SUB socket to tcp://localhost:5556
# Collects weather updates and finds avg temp in zipcode

import sys, zmq

# socket to talk to server
context = zmq.Context()
socket = context.socket(zmq.SUB)

print('collecting updates from weather server ...')
socket.connect('tcp://localhost:5556')

# subscribe to zipcode, default in NYC, 10001
zip_filter = sys.argv[1] if len(sys.argv) > 1 else '10001'

# ascii bytes to unicode str
if isinstance(zip_filter, bytes):
	zip_filter = zip_filter.decode('ascii')

socket.setsockopt_string(zmq.SUBSCRIBE, zip_filter)

#process 5 updates
total_temp = 0
for update_nbr in range(5):
	string = socket.recv_string()
	zipcode, temperature, relhumidity = string.split()
	total_temp += int(temperature)
	
print("average temperature for zipcode '%s' was %dF" % (zip_filter, total_temp / update_nbr))
