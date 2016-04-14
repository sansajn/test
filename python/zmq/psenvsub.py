'''
pubsub envelope subscriber
'''

import zmq

def main():
# Prepare our context and publisher
	context    = zmq.Context()
	subscriber = context.socket(zmq.SUB)
	subscriber.connect("tcp://localhost:5563")
	subscriber.setsockopt(zmq.SUBSCRIBE, b"B")

	while True:
		# Read envelope with address
		[address, contents] = subscriber.recv_multipart()
		print("[%s] %s" % (address, contents))

	# We never get here but clean up anyhow
	subscriber.close()
	context.term()

if __name__ == "__main__":
	main()
