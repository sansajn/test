from shapely.geometry import MultiPoint
points = MultiPoint([(0,0), (1,0), (3,3), (-1,-2), (5,5)])
print(points.bounds)
