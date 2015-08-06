class layer:
	def __init__(self):
		self.name = 'none'

class layer_user(layer):
	def __init__(self):
		layer.__init__(self)
		self.name = 'user layer'


def main():
	lay = layer_user()
	print lay.name


if __name__ == '__main__':
	main()
