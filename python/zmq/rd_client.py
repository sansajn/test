# router <-> dealer client implementacia (asynchronny req/rep)
import zmq

def main():
	ctx = zmq.Context()
	socket = ctx.socket(zmq.DEALER)
	socket.linger = 0
	socket.connect('tcp://localhost:5556')
	
	poller = zmq.Poller()
	poller.register(socket, zmq.POLLIN)
	
	req = 0
	while True:
		if req < 5:
			socket.send('request from DEALER to ROUTER socket %d' % req)
			req += 1

		try:
			items = dict(poller.poll(20))
		except:
			break

		if socket in items:
			d = socket.recv_multipart()
			# TODO: inspect d (items + listing)
			print('response received: %s' % d)

	print('done!')


if __name__ == '__main__':
	main()
