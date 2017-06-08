# pouzitie threading.Lock()
import threading

def do_this():
	global x, lock
	lock.acquire()
	while x < 300:
		x += 1
	print(x)
	lock.release()

def do_after():
	global x, lock
	lock.acquire()
	x = 450
	while x < 600:
		x += 1
	print(x)
	lock.release()

def main():
	global x, lock

	x = 0
	lock = threading.Lock()

	t1 = threading.Thread(target=do_this)
	t1.start()

	t2 = threading.Thread(target=do_after)
	t2.start()

	print(threading.active_count())
	print(threading.enumerate())
	print(threading.current_thread())

	t2.join()

if __name__ == '__main__':
	main()
