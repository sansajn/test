This directory contains (test) coverage sample for CMake and Catch2 library.

Install *gcovr* package with

```bash
sudo apt install gcovr
```

command.

Build with

```bash
cmake -B build-coverage_report -S coverage_report -DCMAKE_BUILD_TYPE=Debug
cmake --build build-coverage_report -j16 -t coverage
```

> **note**: `-DCMAKE_BUILD_TYPE=Debug` needs to be provided to support coverage 

commands.

Coverage report is generated to `build-coverage_report/coverage` direcotry in HTML format and cobertura XML report (`coverage.xml`).

To run unit tests go to the build directory and run `ctest` command this way

```bash
ctest --output-on-failure
```
