Based on [Managing C++ Projects with Conan and CMake](https://www.codeproject.com/Articles/5385907/Managing-Cplusplus-Projects-with-Conan-and-CMake) article from 6.8.2024.

To build `hello_cmake` sample run

```bash
cd hello_cmake
mkdir build && cd build
cmake ..
cmake --build .
```

To build `read_json` sample with conan integration run

```bash
cd read_json
conan install .
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="Release/generators/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

`conan install` command creates `build` directory.