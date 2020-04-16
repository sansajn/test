# Parsing command line arguments with argparse.
# Shows how to define optional '--port N' and optional list of strings as '--filter [STRING, ...]

import argparse


def main():
	parser = argparse.ArgumentParser(description='Parsing command line arguments.')
	parser.add_argument('--port', type=int, default=3333, help='port number')
	parser.add_argument('--filter', nargs='*', default=[], help='filter out specified messages')
	args = parser.parse_args('--filter a b c --port=12'.split())

	print(args)
	
	print('port=%s' % (args.port, ))
	print('filter=%s' % (args.filter, ))

if __name__ == '__main__':
	main()
