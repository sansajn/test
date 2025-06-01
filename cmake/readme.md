# Samples

`hello`: the simpliest CMake project


For something to startup please see sample from `ctest_hello` directory.


library: vygeneruje statickú knižnicu (`add_library`)

`library_output`: statická knižnica zo zmeneným výstupom (`LIBRARY_OUTPUT_PATH`)

`shared_library`: zdielatelná knižnica (`add_library`)

`library_install`: knižnica umožnujúca inštaláciu

`extern_compiled_library`: externá knižnica skompilovaná zo zdrojakov

**use_shared**: program používajúci dynamickú knižnicu, skompilujem príkazom

```bash
$ mkdir build && cd build
$ cmake ..
$ make
```


**shared_with_dependency**: Program volajúci dynamickú knižnicu zo zavislosťou na inej dynamickej knižnici. Ukážku skompilujem príkazom

```bash
$ mkdir build && cd build
$ cmake ..
$ make
```

- `ctest_hello`: Sample with CTest integration for Catch2 library.

```bash
cmake -B build-ctest_hello -S ctest_hello
cmake --build build-ctest_hello -j16
```

- `coverage_report`: Generating coverage report sample for CMake and Catch2 library.

- `sanitizers`: Address sanitizer integration sample (for debug build).

- `clang_tidy`: Clang tidy static analyze tool integration sample.
