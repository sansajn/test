# synchronized subscriber

import time, zmq

def main():
	context = zmq.Context()
	
	# first, connect your subscriber socket
	subscriber = context.socket(zmq.SUB)
	subscriber.connect('tcp://localhost:5561')
	subscriber.setsockopt(zmq.SUBSCRIBE, b'')
	
	time.sleep(1)
	
	# second, synchronize with publisher
	syncclient = context.socket(zmq.REQ)
	syncclient.connect('tcp://localhost:5562')
	
	# send synchronization request
	syncclient.send(b'')
	
	# wait for synchronization reply
	syncclient.recv()
	
	# third, get our updates and report how many we got
	nbr = 0
	while True:
		msg = subscriber.recv()
		if msg == b'END':
			break
		nbr += 1
		
	print('received %d updates' % nbr)

if __name__ == '__main__':
	main()
