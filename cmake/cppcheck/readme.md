# About

This directory contains *CppCheck* analyze integration sample.

Requires *cppcheck* package to be installed with

```bash
sudo apt install cppcheck
```

command.

Build with

```bash
cmake -B build-cppcheck -S cppcheck
cmake --build build-cppcheck -j16 
```

commands.

*CppCheck* complains should be part of the build output, like this

```console
$ cmake --build build-cppcheck
Consolidate compiler generated dependencies of target main
[ 50%] Building CXX object CMakeFiles/main.dir/main.cpp.o
Checking /home/ja/devel/test/cmake/cppcheck/main.cpp ...
/home/ja/devel/test/cmake/cppcheck/main.cpp:24:15: error: Array 'arr[10]' accessed at index 10, which is out of bounds. [arrayIndexOutOfBounds]
 int a10 = arr[10];
              ^
/home/ja/devel/test/cmake/cppcheck/main.cpp:24:12: error: Uninitialized variable: arr [uninitvar]
 int a10 = arr[10];
           ^
```

and despite errors in *CppCheck* output binary file is generated as `build-cppcheck/main`.
