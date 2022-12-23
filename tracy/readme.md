The directory contains Tracy profiler sample with CMake (based on FetchContent).

Build with

```bash
cmake -B build-tracy -S tracy
cmake --build build-tracy -j16
```

> use `--verbose` for verbose build output

The tracy library will be download and saved in `_deps` directory, see

```console
$ ls build-tracy/_deps
external-tracy-build  external-tracy-src  external-tracy-subbuild
```

The Tracy integration was done by *FetchContent* module. To learn more about *FeatchContent* read *Using ExternalProject and FetchContent modules* section in Chapter 7 of *Modern CMake for C++* book.
