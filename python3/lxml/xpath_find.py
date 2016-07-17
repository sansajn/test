# acess xpath html element
from lxml import etree

file_name = 'simple.html'

xpath = '/html/body/b'

fhtml = open(file_name)
parser = etree.HTMLParser()
tree = etree.parse(fhtml, parser)  # vrati ElementTree
result = tree.xpath(xpath)

print('results:%d' % len(result))
print('tag:%s' % result[0].tag)
print('text:%s' % result[0].text)

readable = etree.tostring(tree.getroot(), pretty_print=True, 
	method='html')

print(readable)
