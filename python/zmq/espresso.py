# espresso pattern
import time
from random import randint
from string import uppercase
from threading import Thread
import zmq
from zmq.devices import monitored_queue
from zhelpers import zpipe

# The subscriber thread requests messages starting with
# A and B, then reads and counts incoming messages.
def subscriber_thread():
	ctx = zmq.Context.instance()
	
	# subscribe to A and B
	subscriber = ctx.socket(zmq.SUB)
	subscriber.connect('tcp://localhost:6001')
	subscriber.setsockopt(zmq.SUBSCRIBE, b'A')
	subscriber.setsockopt(zmq.SUBSCRIBE, b'B')
	
	count = 0
	while True:
		try:
			msg = subscriber.recv_multipart()
		except zmq.ZMQError as e:
			if e.errno == zmq.ETERM:
				break
			else:
				raise
		count += 1
	
	print 'Subscriber received %d messages' % count


# publisher thread
# The publisher sends random messages starting with A-J:
def publisher_thread():
	ctx = zmq.Context.instance()
	publisher = ctx.socket(zmq.PUB)
	publisher.bind('tcp://*:6000')
	
	while True:
		string = '%s-%05d' % (uppercase[randint(0,10)], randint(0,100000))
		try:
			publisher.send(string)
		except zmq.ZMQError as e:
			if e.errno == zmq.ETERM:
				break  # interrupted
			else:
				raise
		time.sleep(0.1)


# listener thread
# The listener receives all messages flowing through the proxy, on its
# pipe. Here, the pipe is a pair of ZMQ_PAIR sockets that connects
# attached child threads via inproc. In other languages your mileage may vary:
def listener_thread(pipe):
	while True:
		try:
			print pipe.recv_multipart()
		except zmq.ZMQError as e:
			if e.errno == zmq.ETERM:
				break  # interrupted


# main thread
# The main task starts the subscriber and publisher, and then sets
# itself up as a listening proxy. The listener runs as a child thread:
def main():
	ctx = zmq.Context.instance()
	
	p_thread = Thread(target = publisher_thread)
	s_thread = Thread(target = subscriber_thread)
	p_thread.start()
	s_thread.start()
	
	pipe = zpipe(ctx)
	
	subscriber = ctx.socket(zmq.XSUB)
	subscriber.connect('tcp://localhost:6000')
	
	publisher = ctx.socket(zmq.XPUB)
	publisher.bind('tcp://*:6001')
	
	l_thread = Thread(target = listener_thread, args=(pipe[1],))
	l_thread.start()
	
	try:
		monitored_queue(subscriber, publisher, pipe[0], 'pub', 'sub')
	except KeyboardInterrupt:
		print 'Interrupted'
	
	del subscriber, publisher, pipe
	ctx.term()

if __name__ == '__main__':
	main()
