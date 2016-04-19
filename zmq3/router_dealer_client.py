''' spojenie typu ROUTER <-> DEALER, klient '''
import zmq

def main():
	ctx = zmq.Context()
	socket = ctx.socket(zmq.DEALER)
	socket.linger = 0
	socket.connect('tcp://localhost:5557')

	# ziskaj verziu api
	socket.send('hello')

	# ziskaj odpoved
	while True:
		try:
			d = socket.recv()
		except:
			break
		print('server replied: %s' % d)

	print('done!')

if __name__ == '__main__':
	main()
