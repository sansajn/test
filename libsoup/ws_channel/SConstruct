# dependencies: libsoup2.4-dev
cpp17 = Environment(
    CCFLAGS=['-Wall', '-O0', '-ggdb3'],
    CXXFLAGS=['-std=c++17'])

cpp17.ParseConfig('pkg-config --cflags --libs libsoup-2.4')

cpp17.Program(['test.cpp', 'websocket.cpp', 'glib_event_loop.cpp', 'echo_server.cpp'])
