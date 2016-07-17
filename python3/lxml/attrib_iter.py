'''iteracia attributmi'''
from lxml import etree

file_name = 'simple2.html'

fhtml = open(file_name)
parser = etree.HTMLParser()
tree = etree.parse(fhtml, parser)  # tree:lxml.etree.ElementTree

result = tree.xpath('//*[@id="first_table"]')
table_elem = result[0]  # table_elem:lxml.etree.Element

attribs = table_elem.attrib
for k,v in attribs.items():
	print('%s:%s' % (k,v))

print('done')

