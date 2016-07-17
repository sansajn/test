'''implementacia funkcii find first a find first recursive pre elementy xml/html stromu'''
from lxml import etree

file_name = 'simple2.html'

def main():
	fhtml = open(file_name)
	parser = etree.HTMLParser()
	tree = etree.parse(fhtml, parser)  # tree:lxml.etree.ElementTree

	result = tree.xpath('body')
	body_elem = result[0]  # body_elem:lxml.etree.Element

	# find_first mozem zavolat ako funkciu
	#table_elem = find_first(body_elem, compare_attr)
	
	# alebo ako lambda funkciu
	table_elem = find_first(body_elem, 
		lambda e: 'id' in e.attrib and e.attrib['id'] == 'first_table')

	attribs = table_elem.attrib
	for k,v in attribs.items():
		print('%s:%s' % (k,v))
		
	first_td = find_first_recursive(body_elem,
		lambda e: e.tag == 'td')

	print('first_td.text:', first_td.text)

	print('done')


def compare_attr(elem):
	attrs = elem.attrib
	return 'id' in attrs and attrs['id'] == 'first_table'
	

def find_first(elem, pred):
	for ch in elem:
		if pred(ch):
			return ch
	return None
		
def find_first_recursive(elem, pred):
	for ch in elem:
		if pred(ch):
			return ch
		else:
			if len(ch) > 0:
				res = find_first_recursive(ch, pred)
				if res is not None:
					return res
	return None
	


if __name__ == '__main__':
	main()
