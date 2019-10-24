# returns gcc version number as a triplet
import subprocess as sproc
import re

def main():
	print(gcc_version('gcc'))
	print(gcc_version('gcc-9'))
	print(gcc_version('gcc-9.2'))
	

def gcc_version(gcc):
	try:
		cmd = gcc + ' --version'
		p1 = sproc.Popen(cmd.split(), stdout=sproc.PIPE)
		out = p1.communicate()[0].decode('utf-8')
		version_line = out.split('\n')[0]
		match = re.match('gcc.+(\d)\.(\d)\.(\d).*', version_line)
		ver = match.group(1, 2, 3)  # ('5', '4', '0')
		return (int(ver[0]), int(ver[1]), int(ver[2]))
	except FileNotFoundError:
		return (0, 0, 0)


if __name__ == '__main__':
	main()
