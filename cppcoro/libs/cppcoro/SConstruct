# dependencies, libc++-dev (10+), libc++abi-dev (10+), clang (10+)

cpp20 = Environment(
	CXX='clang++-10',
	CCFLAGS=['-std=c++20', '-Wall', '-O0', '-g', '-fcoroutines-ts', '-stdlib=libc++'],
	CPPPATH=['include'],
	CPPDEFINES=['CPPCORO_OS_LINUX'],
	LINKFLAGS=['-stdlib=libc++'],
	LIBS=['pthread'])

exclude_files = [  # file wont compile under linux (Ubuntu 20.04 LTS)
	'lib/file.cpp',
	'lib/file_read_operation.cpp',
	'lib/io_service.cpp',
	'lib/read_only_file.cpp',
	'lib/read_write_file.cpp',
	'lib/readable_file.cpp',
	'lib/socket.cpp',
	'lib/socket_accept_operation.cpp',
	'lib/socket_connect_operation.cpp',
	'lib/socket_disconnect_operation.cpp',
	'lib/socket_recv_from_operation.cpp',
	'lib/socket_recv_operation.cpp',
	'lib/socket_send_operation.cpp',
	'lib/socket_send_to_operation.cpp',
	'lib/win32.cpp',
	'lib/writable_file.cpp',
	'lib/write_only_file.cpp']

cppcoro = cpp20.StaticLibrary('cppcoro',
	[f for f in Glob('lib/*.cpp') if not str(f) in exclude_files])


# build some tests

cpp20.Program('test/test', [
	'test/counted.cpp',
	'test/main.cpp',
	'test/task_tests.cpp',
	cppcoro])

cpp20.Program(['hello.cpp', cppcoro])

cpp20.Program(['temp_test.cpp', cppcoro])

cpp20.Program(['task1.cpp', cppcoro])
