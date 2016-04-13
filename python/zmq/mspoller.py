# reading from multiple sockets
# this version uses zmq.Poller()

import zmq, time

context = zmq.Context()

# connect to the task ventiator (umozni prijimanie sprav od ventilatora)
receiver = context.socket(zmq.PULL)
receiver.connect('tcp://loacalhost:5557')

# connect to weather server
subscriber = context.socket(zmq.SUB)
subscriber.connect('tcp://localhost:5556')
subscriber.setsockopt(zmq.SUBSCRIBE, b'10001')

# initialize pool set
poller = zmq.Poller()
poller.register(receiver, zmq.POLLIN)
poller.register(subscriber, zmq.POLLIN)

# process messages from both sockets
while True:
	try:
		socks = dict(poller.poll())
	except KeyboardInterrupt:
		break

	if receiver in socks:
		message = receiver.recv()
		# process task from ventilator
		
	if subscriber in socks:
		message = subscriber.recv()
		# process weather update

print('done')
