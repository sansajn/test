'prienik dvoch objektou'
from shapely.geometry import Point

a = Point(1,1).buffer(1.5)  # 1.5 radius circle with (1,1) center
b = Point(2,1).buffer(1.5)
common = a.intersection(b)

print('intersect: %s' % (not common.is_empty, ))
