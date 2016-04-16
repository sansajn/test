'''
Multithreaded Hello World server
'''

import time, threading, zmq

def worker_routine(worker_url, context=None):
	context = context or zmq.Context.instance()
	# socket ot talk to dispatcher
	socket = context.socket(zmq.REP)
	socket.connect(worker_url)
	while True:
		s = socket.recv()
		print('received request: [%s]' % (s,))
		time.sleep(1)
		socket.send(b'World')
		
def main():
	url_worker = 'inproc://workers'
	url_client = 'tcp://*:5555'
	
	# prepare our context and sockets
	context = zmq.Context.instance()
	
	# socket to talk to clients
	clients = context.socket(zmq.ROUTER)
	clients.bind(url_client)
	
	# socket to talk to walkers
	workers = context.socket(zmq.DEALER)
	workers.bind(url_worker)
	
	# launch pool of worker threads
	for i in range(5):
		thread = threading.Thread(target=worker_routine, args=(url_worker, ))
		thread.start()
		
	zmq.proxy(clients, workers)
	
	# we never get there but clean up anyhow
	clients.close()
	workers.close()
	context.term()
	
if __name__ == '__main__':
	main()
