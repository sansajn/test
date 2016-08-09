# 

def multiply():
	c = 12345*6789
	print('The result of 12345 x 6789 = %d' % (c,))
	return c

def multiply_with_arg(x):
	c = x*x
	print('The result of %g x %g = %g' % (x, x, c))
	return c

if __name__ == '__main__':
	multiply()
