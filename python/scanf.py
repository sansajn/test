def scanf(s, delim, fmt):
	return [f(c) for c,f in zip(s.split(delim), fmt)]

def main():
	s = '1, 2, 3.23'
	print scanf(s, ',', [int, int, float])

main()
