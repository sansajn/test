# libglfw3-dev (3.2.1, ubuntu 18.04)

cpp17 = Environment(
	CCFLAGS=['-std=c++17', '-Wall', '-O0', '-g'],
	CPPDEFINES=['GLFW_INCLUDE_ES2'])

cpp17.ParseConfig('pkg-config --cflags --libs glfw3 glesv2')

phys = cpp17.Object(Glob('phys/*.cpp'))

cpp17.Program('triangle.cpp')
cpp17.Program('xy_plane.cpp')
cpp17.Program(['row_major_xy_plane.cpp', phys])
