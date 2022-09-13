This directory contains samples for Boost.Stacktrace library.

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
 
