This directory contains Clang Tidy analyze integration.

Requires *clang-tidy* package to be installed with

```bash
sudo apt install clang-tidy
```

command.

Build with

```bash
cmake -B build-clang_tidy -S clang_tidy
cmake --build build-clang_tidy -j16 
```

commands.

Clang tidy complains should be part of the build output, like this

```console
$ cmake --build build-clang_tidy/ -j16
[ 50%] Building CXX object CMakeFiles/main.dir/main.cpp.o
/home/ja/devel/test/cmake/clang_tidy/main.cpp:2:1: warning: #includes are not sorted properly [llvm-include-order]
#include <catch.hpp>
^
note: this fix will not be applied because it overlaps with another fix
/home/ja/devel/test/cmake/clang_tidy/main.cpp:2:1: warning: system include catch.hpp not allowed [llvmlibc-restrict-system-libc-headers]
#include <catch.hpp>
^~~~~~~~~~~~~~~~~~~~
/home/ja/devel/test/cmake/clang_tidy/main.cpp:7:24: warning: variable name 'q' is too short, expected at least 3 characters [readability-identifier-length]
        concurrent_queue<int> q;
                              ^
/home/ja/devel/test/cmake/clang_tidy/main.cpp:11:9: warning: 5 is a magic number; consider replacing it with a named constant [cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers]
        q.push(5);
               ^
/home/ja/devel/test/cmake/clang_tidy/main.cpp:14:6: warning: variable 'task' is not initialized [cppcoreguidelines-init-variables]
        int task;
            ^
                 = 0
```
