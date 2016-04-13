# Task ventilator
# Binds PUSH socket to tcp://localhost:5557
# Sends batch of tasks to workers via that socket

import zmq, random, time

try:
	raw_input
except NameError:
	# python 3
	raw_input = input

context = zmq.Context()

# socket to send messages on
sender = context.socket(zmq.PUSH)
sender.bind('tcp://*:5557')

# socket with direct access to the sink: used to synchronize start of batch
sink = context.socket(zmq.PUSH)
sink.connect('tcp://localhost:5558')

print('press enter when the workers are ready: ')
_ = raw_input()
print('sending tasks to workers ...')

sink.send(b'0')  # the first message is '0' and signals start of batch

random.seed()

# send 100 task
total_msec = 0
for task_nbr in range(100):
	workload = random.randint(1, 100)
	total_msec += workload
	sender.send_string(u'%i' % workload)
	
print('total expected cost: %s msec' % total_msec)

time.sleep(1)  # give 0MQ time to deliver

