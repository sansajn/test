'prienik dvoch objektou pomocou itersects'
from shapely.geometry import Point, Polygon

a = Polygon([
	(0,0),
	(1,0),
	(1,1),
	(0,1),
	(0,0)
])

b = Point(1,1).buffer(1)  # kruh zo stredom v (1,1) a polomerom 1

print('intersects: %s' % (a.intersects(b),))
