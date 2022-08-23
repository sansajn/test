This directory contains sanitizers integration sample.

Build with

```bash
cmake -B build-Debug-sanitizers -S sanitizers -DCMAKE_BUILD_TYPE=Debug
cmake --build build-Debug-sanitizers -j16 
```

> **note**: `-DCMAKE_BUILD_TYPE=Debug` needs to be provided to build with sanitizers (address) support

commands.

To demonstarte sanitizers are working, run

```console
$ ./build-Debug-sanitizers/sanitizer
===============================================================================
All tests passed (5 assertions in 2 test cases)


=================================================================
==37454==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 4 byte(s) in 1 object(s) allocated from:
    #0 0x7fb42522c1c7 in operator new(unsigned long) ../../../../src/libsanitizer/asan/asan_new_delete.cpp:99
    #1 0x55afd6cd2b8b in ____C_A_T_C_H____T_E_S_T____2 /home/ja/devel/test/cmake/sanitizers/sanitizer.cpp:23
```

command after successfull build. To exclude *memory-leak* affected unit test run

```console
$ ./build-Debug-sanitizers/sanitizer ~[memory-leak]
===============================================================================
All tests passed (5 assertions in 1 test case)
```

command.
