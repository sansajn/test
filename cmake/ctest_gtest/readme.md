Configure and build with

```bash
cmake -S ctest_gtest -B build-ctest_gtest
cmake --build build-ctest_gtest -j4
```

commands from outside of `ctest_gtest` directory.

Run tests with

```bash
ctest --test-dir build-ctest_gtest --output-on-failure
```

command.
