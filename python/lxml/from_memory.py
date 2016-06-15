from lxml import etree
from io import StringIO, BytesIO

html = '<html></html>'
parser = etree.HTMLParser()
tree = etree.parse(StringIO(html), parser)
result = etree.tostring(tree.getroot(), pretty_print=True, method='html')

print(result)
