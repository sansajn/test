'''
Synchronized publisher
'''

import zmq

SUBSCRIBERS_EXPECTED = 10

def main():
	context = zmq.Context()
	
	# socket to talk to clients
	publisher = context.socket(zmq.PUB)
	
	# set SNDHWM, so we don't drop messages for slow subscribers
	publisher.sndhwm = 1100000
	publisher.bind('tcp://*:5561')
	
	# socket to receive signals
	syncservice = context.socket(zmq.REP)
	syncservice.bind('tcp://*:5562')
	
	# get synchronication from subscribers
	subscribers = 0
	while subscribers < SUBSCRIBERS_EXPECTED:
		# wait for synchronizationi request
		s = syncservice.recv()
		# send synchronization reply
		syncservice.send(b'')
		subscribers += 1
		print '+1 subscriber (%i/%i)' % (subscribers, SUBSCRIBERS_EXPECTED)
		
	# now broadcast exactly 1M updates followed by END
	for i in range(1000000):
		publisher.send(b'Rhubarb')
		
	publisher.send(b'END')
	
if __name__ == '__main__':
	main()
