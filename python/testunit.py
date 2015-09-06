import unittest

class test_foo(unittest.TestCase):
	def setUp(self):
		self.tested = (1, 2, 3)

	def test_content(self):
		self.assertEqual(self.tested, (1, 2, 3))

	def test_contains(self):
		self.assertTrue(2 in self.tested)

if __name__ == '__main__':
	unittest.main()

