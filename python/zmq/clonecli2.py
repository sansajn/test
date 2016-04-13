# clone client model two
import time, zmq
from kvsimple import KVMsg

def main():
	# prepare our context for subscriber
	ctx = zmq.Context()
	snapshot = ctx.socket(zmq.DEALER)
	snapshot.linger = 0
	snapshot.connect('tcp://localhost:5556')
	subscriber = ctx.socket(zmq.SUB)
	subscriber.linger = 0
	subscriber.setsockopt(zmq.SUBSCRIBE, '')
	subscriber.connect('tcp://localhost:5557')
	
	kvmap = {}
	
	# get state snapshot
	sequence = 0
	snapshot.send('ICANHAZ?')
	while True:
		try:
			kvmsg = KVMsg.recv(snapshot)
		except KeyboardInterrupt:
			break
		
		if kvmsg.key == 'KTHXBAI':
			sequence = kvmsg.sequence
			print 'Received snapshot = %d' % sequence
			break  # done
		kvmsg.store(kvmap)
		
	print 'state snapshot received'
		
	# now apply pending updates, discard out-of-sequence messages
	while True:
		try:
			kvmsg = KVMsg.recv(subscriber)
		except KeyboardInterrupt:
			break
		if kvmsg.sequence > sequence:
			sequence = kvmsg.sequence
			kvmsg.store(kvmap)

if __name__ == '__main__':
	main()
