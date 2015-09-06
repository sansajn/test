import threading

def foo():
	print 'Hello!'

def main():
	t = threading.Timer(0.5, foo)
	t.start()

if __name__ == '__main__':
	main()
