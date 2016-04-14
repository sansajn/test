'''
Pubsub envelope publisher
'''

import time, zmq

def main():

	# prepare our context and publisher
	context = zmq.Context()
	publisher = context.socket(zmq.PUB)
	publisher.bind('tcp://*:5563')
	
	while True:
		# write two messages, each with an envelope and content
		publisher.send_multipart([b'A', b"We don't want to see this"])
		publisher.send_multipart([b'B', b"We would like to see this"])
		time.sleep(1)
		
	publisher.close()
	context.term()
	
if __name__ == '__main__':
	main()


