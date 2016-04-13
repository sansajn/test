'''
Simple message queuing broker
Same as request-reply broker but using zmq.proxy
Spustaj spolu s rrclient.py a rrworker.py
'''

import zmq

def main():
	context = zmq.Context()
	
	# socket facing clients
	frontend = context.socket(zmq.ROUTER)
	frontend.bind('tcp://*:5559')
	
	# socket facing services
	backend = context.socket(zmq.DEALER)
	backend.bind('tcp://*:5560')
	
	zmq.proxy(frontend, backend)  # presmeruje komuniikaciu od klienta k service a naopak
	
	# we never get here ...
	frontend.close()
	backend.close()
	context.term()

if __name__ == '__main__':
	main()
