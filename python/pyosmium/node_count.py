import osmium

class CounterHandler(osmium.SimpleHandler):
	def __init__(self):
		osmium.SimpleHandler.__init__(self)
		self.num_nodes = 0

	def node(self, n):
		self.num_nodes += 1

def main():
	h = CounterHandler()
	h.apply_file('atm.osm')
	print('number fo nodes: %d' % h.num_nodes)

if __name__ == '__main__':
	main()
