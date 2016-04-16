'''
Multithreaded relay
'''

import threading, zmq

def step1(context=None):
	context = context or zmq.Context.instance()
	# signal downstream to sleep 2
	sender = context.socket(zmq.PAIR)
	sender.connect('inproc://step2')
	sender.send(b'')

def step2(context=None):
	context = context or zmq.Context.instance()
	# bind to inproc: endpoint, then start upstream thread
	receiver = context.socket(zmq.PAIR)
	receiver.bind('inproc://step2')
	
	thread = threading.Thread(target=step1)
	thread.start()
	
	# wait for signal
	msg = receiver.recv()
	
	# signal downstream to step 3
	sender = context.socket(zmq.PAIR)
	sender.connect('inproc://step3')
	sender.send(b'')
	
def main():
	context = zmq.Context.instance()
	
	# bind to inproc: endpoint, then start upstream thread
	receiver = context.socket(zmq.PAIR)
	receiver.bind('inproc://step3')
	
	thread = threading.Thread(target = step2)
	thread.start()
	
	# wait for signal
	s = receiver.recv()
	
	print('test successful!')
	
	receiver.close()
	context.term()
	
if __name__ == '__main__':
	main()
