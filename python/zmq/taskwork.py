# Task worker
# Connects PULL socket to tcp://localhost:5557
# Collects workloads from ventilator via that socket
# Connects PUSH socket to tcp://localhost:5558
# Sends results to sink via that socket

import sys, time, zmq

context = zmq.Context()

# socket to receive messages on
receiver = context.socket(zmq.PULL)
receiver.connect('tcp://localhost:5557')

# socket to send messsages to
sender = context.socket(zmq.PUSH)
sender.connect('tcp://localhost:5558')

# process tasks forrever
while True:
	s = receiver.recv()
	
	# simple process indicator for a viewer
	sys.stdout.write('.')
	sys.stdout.flush()
	
	# do the workerk
	time.sleep(int(s)*0.001)
	
	# send results to sink
	sender.send(b'')
