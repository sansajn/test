class foo:
	def __init__(self):
		self.d = range(5)
	def get(self, i):
		return self.d[i]


f=foo
print f.d
f.get(3)=9
print f.d
f.get(2)=4
print f.d 

