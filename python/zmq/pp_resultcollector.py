# push/pull result collector
import time
import zmq
import pprint

def result_collector():
	context = zmq.Context()
	result_receiver = context.socket(zmq.PULL)
	result_receiver.bind('tcp://127.0.0.1:5558')
	collecter_data = {}
	for x in xrange(1000):
		result = result_receiver.recv_json()
		if collecter_data.has_key(result['consumer']):
			collecter_data[result['consumer']] = collecter_data[result['consumer']] + 1  # obsah data[X] o +1
		else:
			collecter_data[result['consumer']] = 1
		if x == 999:
			pprint.pprint(collecter_data)
			
result_collector()
