# About

The directory contains samples for Boost.Stacktrace library.

The Stacktrace library can be configured in a various way by `BOOST_STACKTRACE_USE_BACKTRACE` or `BOOST_STACKTRACE_USE_ADDR2LINE` macros.

> TODO: check whether there are some more *USE* macros in stacktrace library

It looks like that using `BOOST_STACKTRACE_USE_ADDR2LINE` provide more detailed stacktrace e.g.

```console
19# gst_element_change_state in /lib/x86_64-linux-gnu/libgstreamer-1.0.so.0
20# 0x00007FCF1415960E in /lib/x86_64-linux-gnu/libgstreamer-1.0.so.0
21# destroy_receiver_entry(void*) in ./single_webrtc
22# websocket_closed_handler(_SoupWebsocketConnection*, void*) in ./single_webrtc
23# ffi_call_unix64 in /lib/x86_64-linux-gnu/libffi.so.6
24# ffi_call in /lib/x86_64-linux-gnu/libffi.so.6
25# g_cclosure_marshal_generic in /lib/x86_64-linux-gnu/libgobject-2.0.so.0
26# g_closure_invoke in /lib/x86_64-linux-gnu/libgobject-2.0.so.0
```

compared to `BOOST_STACKTRACE_USE_BACKTRACE` which produce

```console
8# 0x00007FF9E5B3AF80 in /lib/x86_64-linux-gnu/libgstreamer-1.0.so.0
 9# 0x00007FF9E5B3B60E in /lib/x86_64-linux-gnu/libgstreamer-1.0.so.0
10# destroy_receiver_entry(void*) at /home/ja/devel/test/gstreamer/webrtc/single_webrtc.cpp:391
11# websocket_closed_handler(_SoupWebsocketConnection*, void*) at /home/ja/devel/test/gstreamer/webrtc/single_webrtc.cpp:296
12# 0x00007FF9E4BC38EE in /lib/x86_64-linux-gnu/libffi.so.6
13# 0x00007FF9E4BC32BF in /lib/x86_64-linux-gnu/libffi.so.6
14# 0x00007FF9E5820492 in /lib/x86_64-linux-gnu/libgobject-2.0.so.0
15# 0x00007FF9E581FC9D in /lib/x86_64-linux-gnu/libgobject-2.0.so.0
```

> this is true at least under *Debian 10*, but can vary on other platforms ....


Output from *Debian 10* (boost 1.67) compiled with `BOOST_STACKTRACE_USE_BACKTRACE` (`boost_stacktrace_backtrace`, `dl`, `backtrace`) option

```console
$ ./exception_with_stacktrace 
terminate called after throwing an instance of 'custom_error'
  what():  the error message

Stack trace:
 0# custom_error::custom_error(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) at /home/ja/devel/exception_with_stacktrace.cpp:16
 1# main at /home/ja/devel/exception_with_stacktrace.cpp:31
 2# 0x00007F54F908109B in /lib/x86_64-linux-gnu/libc.so.6
 3# 0x00005629ABD9430A in ./exception_with_stacktrace
```

the same output from *Kubuntu 20.04 LTS* (boost 1.71)

```console
$ ./exception_with_stacktrace 
terminate called after throwing an instance of 'custom_error'
  what():  the error message

Stack trace:
 0# custom_error::custom_error(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) at /home/hlavatovic/devel/test/boost/stacktrace/exception_with_stacktrace.cpp:16
 1# main at /home/hlavatovic/devel/test/boost/stacktrace/exception_with_stacktrace.cpp:31
 2# __libc_start_main at ../csu/libc-start.c:342
 3# _start in ./exception_with_stacktrace
 ```
 
