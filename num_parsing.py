import re

integer_expr = r'-?\d+'
float_expr = r'-?(?:\d+\.\d+|\d*\.\d+)'
scient_expr = r'-?\d(?:\.\d+|)[Ee][+\-]?\d\d*'
real_expr = r'(?:' + scient_expr + '|' + float_expr + '|' + integer_expr + r')'

integers = ['12', '-4', '07']
floats = ['12.32', '-1.12', '.12', '-.34'] 
scientifics = ['1.4e4', '1e-4', '1.3E+7', '-1.23e+5', '5e-005']

def main():
	print 'Integer test ...'
	if do_test(integers, integer_expr):
		print 'passed!'

	print '\nFloat test ...'
	if do_test(floats, float_expr):
		print 'passed!'

	print '\nReal test ...'
	data = []
	data[:] = integers
	data.extend(floats)
	if do_test(data, real_expr):
		print 'passed!'

	print '\nScientifics test ...'
	if do_test(scientifics, scient_expr):
		print 'passed!'


def do_test(inputs, expr):
	for n in inputs:
		ma = re.match(expr, n)
		if not ma:
			print "failed to parse '%s'" % n
			print 'failed!'
			return False
		else:
			print ma.group(0)
	return True

if __name__ == '__main__':
	main()
