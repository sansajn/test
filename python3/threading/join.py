# pouzitie Thread.join() funkcie
import threading

def do_this():
	print('This is our thread!')
	x = 0
	while x < 1e8:
		x += 1
	print('thread done')

def main():
	t = threading.Thread(target=do_this, name='test thread')
	t.start()

	print(threading.active_count()) 
	print(threading.enumerate())
	print(threading.current_thread())

	t.join()

if __name__ == '__main__':
	main()
