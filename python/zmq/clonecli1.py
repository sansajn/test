# clone client model one
import random, time, zmq
from kvsimple import KVMsg

def main():
	ctx = zmq.Context()
	updates = ctx.socket(zmq.SUB)
	updates.linger = 0
	updates.setsockopt(zmq.SUBSCRIBE, '')
	updates.connect('tcp://localhost:5556')
	kvmap = {}
	sequence = 0
	while True:
		try:
			kvmsg = KVMsg.recv(updates)
		except KeyboardInterrupt:
			break  # interrupted
		kvmsg.store(kvmap)
		sequence += 1
	print 'Interrupted\n%d messages in' % sequence

if __name__ == '__main__':
	main()
