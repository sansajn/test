Configure and build with

```bash
cmake -S gtest -B build-gtest
cmake --build build-gtest -j4
```

commands from outside of `gtest` directory.

Run tests with

```bash
ctest --test-dir build-gtest --output-on-failure
```

command.
