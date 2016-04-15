# clone server model one
import random, time, zmq
from kvsimple import KVMsg

def main():
	# prepare our context and publisher socket
	ctx = zmq.Context()
	publisher = ctx.socket(zmq.PUB)
	publisher.bind('tcp://*:5556')
	time.sleep(0.2)
	random.seed(time.time())

	kvmap = {}
	sequence = 0

	try:
		while True:
			# distribute as key-value message
			sequence += 1
			kvmsg = KVMsg(sequence)
			kvmsg.key = '%d' % random.randint(1, 10000)
			kvmsg.body = '%d' % random.randint(1, 1000000)
			kvmsg.send(publisher)
			kvmsg.store(kvmap)
	except KeyboardInterrupt:
		print ' Interrupted\n%d messages out' % sequence

if __name__ == '__main__':
	main()
