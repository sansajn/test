import threading

def do_this():
	global dead
	print('This is our thread!')
	while not dead:
		pass

def main():
	global dead
	dead = False

	t = threading.Thread(target=do_this, name='test thread')
	t.start()

	print(threading.active_count()) 
	print(threading.enumerate())
	print(threading.current_thread())

	raw_input('Hit enter to die.')
	dead = True

if __name__ == '__main__':
	main()
