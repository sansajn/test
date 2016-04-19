''' spojenie typu ROUTER <-> DEALER, server (umoznujuce asynchronny
pokec typu req/rep)
'''
import zmq

def main():
	ctx = zmq.Context()
	socket = ctx.socket(zmq.ROUTER)
	socket.bind('tcp://*:5557')

	# cakaj na dotazy
	while True:
		try:
			identity, d = socket.recv_multipart()
		except:
			break

		print('received "%s"' % d)
		socket.send(identity, zmq.SNDMORE)
		socket.send('world')

	print('done!')

if __name__ == '__main__':
	main()
