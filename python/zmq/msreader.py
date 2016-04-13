# reading from multiple sockets
# this version uses a simple recv loop

import zmq, time

context = zmq.Context()

# connect to the task ventiator (umozni prijimanie sprav od ventilatora)
receiver = context.socket(zmq.PULL)
receiver.connect('tcp://loacalhost:5557')

# connect to weather server
subscriber = context.socket(zmq.SUB)
subscriber.connect('tcp://localhost:5556')
subscriber.setsockopt(zmq.SUBSCRIBE, b'10001')

# process messages from both sockets
# we orioritize traffic from the task ventilator
while True:

	# process any waiting tasks
	while True:
		try:
			msg = receiver.recv(zmq.DONTWAIT)
		except zmq.Again:
			break
		
		# process task
		
	# process any waiting weather update
	while True:
		try:
			msg = subscriber.recv(zmq.DONTWAIT)
		except zmq.Again:
			break
		# process weather update

	time.sleep(0.001)



