env = Environment(
	CXX='clang++', CXXFLAGS=['-std=c++14'], CCFLAGS=['-Wall', '-g', '-O0'])

import os  # enable color in output
if 'TERM' in os.environ:
	env['ENV']['TERM'] = os.environ['TERM']

env.Program('set_container.cpp')
env.Program('set_container_sort.cpp')
env.Program('listener_function.cpp')
env.Program('listener_visitor.cpp')
env.Program('istream_iterator_file.cpp')
env.Program('path_tail.cpp')
env.Program('game_fsm.cpp')
env.Program('game_fsm2.cpp')
env.Program('crtp.cpp')
