import math

class mercator:

	@staticmethod
	def torad(x):
		return mercator.deg2rad*x

	@staticmethod
	def todeg(x):
		return mercator.rad2deg*x

	rad2deg = 180.0/math.pi
	deg2rad = math.pi/180.0
	pio2 = math.pi/2.0
	pio4 = math.pi/4.0
	pio360 = math.pi/360.0


def gps2xy(gpos, zoom):
	'zdroj: http://wiki.openstreetmap.org/wiki/Mercator'
	x = gpos[1]
	lat = gpos[0]
	y = mercator.todeg(
		math.log(math.tan(mercator.pio4+mercator.pio360*lat))
	)	
	return (x, y)

def old_gps2xy(gpos, zoom):
	'\param gpos is a point type'
	lat,lon = (gpos[0], gpos[1])
	n = 2**zoom*256
	lat_rad = math.radians(lat)
	x = int((lon+180.0)/360.0 * n)
	y = int((1.0 - math.log(math.tan(lat_rad)+(1/math.cos(lat_rad)))
		/ math.pi) / 2.0*n)
	return (x, y)

def old2_gps2xy(gpos, zoom):
	lat,lon = (gpos[0], gpos[1])
	lat_rad = math.radians(lat)
	x = (lon+180.0)/360.0
	y = (1.0 - math.log(math.tan(lat_rad)+(1/math.cos(lat_rad))) / math.pi) / 2.0
	n = 2**zoom*256
	return (int(n*x+.5), int(n*y+.5))

def old_gps2xybase(gpos, zoom):
	lat,lon = (gpos[0], gpos[1])
	lat_rad = math.radians(lat)
	x = (lon+180.0)/360.0
	y = (1.0 - math.log(math.tan(lat_rad)+(1/math.cos(lat_rad))) / math.pi) / 2.0	
	return (x, y)

#def y2lat(a):
#	return 180.0/math.pi*(2.0*math.atan(math.exp(a*math.pi/180.0))-math.pi/2.0)

def lat2y(a):
	return 180.0/math.pi*math.log(math.tan(math.pi/4.0+a*(math.pi/180.0)/2.0))


def main():
	zoom = 10
	bratislava = (48.143889, 17.109722)
	print('bratislava:%s' % (str(bratislava),))
	print gps2xy(bratislava, 0)
	print('old_gps2xy():{}'.format(old_gps2xy(bratislava, zoom)))
	print('old2_gps2xy():{}'.format(old2_gps2xy(bratislava, zoom)))
	print('old_gps2xybase():{}'.format(old_gps2xybase(bratislava, zoom)))
	print('osm:{}'.format(lat2y(bratislava[0])))
	print("6130826,1904646")


if __name__ == '__main__':
	main()
