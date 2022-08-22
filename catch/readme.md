This directory contains folloving

- `hello.cpp`: Catch 2 (v2) hello sample.

Catch2 related samples.

Build with

```bash
cmake -B build-catch -S catch
cmake --build build-catch -j16
```

commands.

For a demonstration purpose unit tests for `hello` target are "exported" see

```cmake
enable_testing()

# ...

include(Catch)
catch_discover_tests(hello)
```

parts of build script. To run unit tests go to the build directory and run `ctest` command this way

```console
~/devel/test/build-catch$ ctest --output-on-failure
Test project /home/ja/devel/test/build-catch
    Start 1: vectors can be sized and resized
1/1 Test #1: vectors can be sized and resized ...   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```
