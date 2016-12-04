# downloads osm tiles
import urllib2, os

DEPTH=9

def main():
	if not os.path.exists('tiles'):
		os.mkdir('tiles')

	for level in range(0, DEPTH):
		print('downloading level %d tiles (%d) ...' % (level, 2**level * 2**level))
		for x in range(0, 2**level):
			if not os.path.exists('tiles/%d/%d' % (level,x)):
				os.makedirs('tiles/%d/%d' % (level,x))
				
			for y in range(0, 2**level):
				url = construct_tile_url(x, y, level)
				download_tile(url, 'tiles/%d/%d/%d.png' % (level, x, y))

def construct_tile_url(x, y, z):
	return 'http://tile.openstreetmap.org/%d/%d/%d.png' % (z, x, y)

def download_tile(url, fname):
	if os.path.exists(fname):
		return
	else:
		ftile = urllib2.urlopen(url)
		fout = open(fname, 'wb')
		fout.write(ftile.read())

if __name__ == '__main__':
	main()
