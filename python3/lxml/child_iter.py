'''iteracia potomkami elementu'''
from lxml import etree
from io import StringIO

file_name = 'simple2.html'

fhtml = open(file_name)
parser = etree.HTMLParser()
tree = etree.parse(fhtml, parser)  # tree:lxml.etree.ElementTree
print('tree:', type(tree))

root = tree.getroot()  # root:lxml.etree.Element
print('root:', type(root))

result = root.xpath('body')  # []
print('result:', type(result))
print('results:', len(result))

body_elem = result[0]  # body_elem:lxml.etree.Element
print('body_elem:', type(body_elem))

for child in body_elem:
	print(child.tag)

print('done')
