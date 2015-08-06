class layer:
	def __init__(self):
		print 'layer.__init__()'

	def a(self):
		print 'layer.a()'


class layer_user(layer):
	def __init__(self):
		print 'layer_user.__init__()'

	def a(self):
		layer.a(self)
		print 'layer_user.a()'


def main():
	lay = layer_user()
	lay.a()


if __name__ == '__main__':
	main()
