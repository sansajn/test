# queue

import zmq

def main():
	try:
		context = zmq.Context(1)
		# socket facing clients
		frontend = context.socket(zmq.XREP)
		frontend.bind('tcp://*:5559')
		# socket facing service
		backend = context.socket(zmq.XREQ)
		backend.bind('tcp://*:5560')
		
		zmq.device(zmq.QUEUE, frontend, backend)  # blokovacie volanie ?
	except Exception, e:
		print e
		print 'bringing down zmq device'
	finally:
		pass
		frontend.close()
		backend.close()
		context.term()
		
if  __name__ == '__main__':
	main()

