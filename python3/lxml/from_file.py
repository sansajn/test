# parse from file
from lxml import etree
from io import StringIO, BytesIO

file_name = 'simple.html'

fhtml = open(file_name)
parser = etree.HTMLParser()
tree = etree.parse(fhtml, parser)
result = etree.tostring(tree.getroot(), pretty_print=True, method='html')

print(result)
