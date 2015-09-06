import math

def direction_from_camera_2d(C, O):	
	d = (C[0]-O[0], C[1]-O[1])  # C-O
	distance = math.sqrt(d[0]*d[0] + d[1]*d[1])
	angle = math.atan2(d[1], d[0])
	return (distance, angle)


camera_pos = (2, 3)
object_pos = (4, 2)

dir = direction_from_camera_2d(camera_pos, object_pos)
print 'object distance:%g, angle:%g' % (dir[0], math.degrees(dir[1]))

