# push/pull pattern
import time
import zmq

def producer():
	context = zmq.Context()
	zmq_socket = context.socket(zmq.PUSH)
	zmq_socket.bind('tcp://127.0.0.1:5557')
	for num in xrange(20000):
		work_message = {'num':num}
		zmq_socket.send_json(work_message)
		
producer()
