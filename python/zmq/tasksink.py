# Task sink
# Binds PULL socket to tcp://localhost:5558
# Collects results from workers via that socket

import sys, time, zmq

context = zmq.Context()

# socket to receive messages on
receiver = context.socket(zmq.PULL)
receiver.bind('tcp://*:5558')

# wait for start of batch
s = receiver.recv()

# start our clock now
tstart = time.time()

# process 100 confirmations
total_msec = 0
for task_nbr in range(100):
	s = receiver.recv()
	if task_nbr % 10 == 0:
		sys.stdout.write(':')
	else:
		sys.stdout.write('.')
	sys.stdout.flush()
	
tend = time.time()
print('total elapsed time: %d msec' % ((tend-tstart)*1000))
