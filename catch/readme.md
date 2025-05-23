This directory contains folloving

- `event_listener`
- `exception_testing`
- `floats`
- `sections`

Catch2 (not sure whether for v2 or even older) reated samples managed by Scon build tool.

Run e.g.

```bash
scons floats
```

to build `floats` sample.

In Ubuntu 24.04 LTS Catch2 version 3 was introduced so in a `v3.4.0` drectory there are following CMake based samples with (Catch library vrsion check)

- `capture`: Evaluate values in failed test with `CAPTURE()` function.


For previous 22.04 LTS ubuntu check `v2.13.8` drectory with

- `hello.cpp`: Catch 2 (v2) hello sample.

Catch2 related samples.

Build with

```bash
cd path/to/catch
cmake -B build-v3.4.0 -S v3.4.0
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
