# router <-> dealer server implementacia (asynchronny req/rep)
import zmq

def main():
	ctx = zmq.Context()
	socket = ctx.socket(zmq.ROUTER)  # vo funkcii rep soketu
	socket.bind('tcp://*:5556')
	
	while True:
		try:
			identity, d = socket.recv_multipart()
		except:
			break
		print('request received: %s' % d)
		
		# make response
		socket.send(identity, zmq.SNDMORE)
		socket.send('server response')
		
	print('done!')

if __name__ == '__main__':
	main()
