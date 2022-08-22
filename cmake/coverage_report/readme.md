This directory contains (test) coverage sample for CMake and Catch2 library.

Install *lcov* package with

```bash
sudo apt install lcov
```

command.

Build with

```bash
cmake -B build-coverage_report -S coverage_report
cmake --build build-coverage_report -j16 -DCMAKE_BUILD_TYPE=Debug
```

> **note**: `-DCMAKE_BUILD_TYPE=Debug` needs to be provided to support coverage 

commands.

To run unit tests go to the build directory and run `ctest` command this way

```bash
ctest --output-on-failure
```
