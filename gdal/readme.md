Install dependencies with

```
apt install libgdal-dev
```

command.

Build with

```bash
cmake -B build-gdal -S gdal
cmake --build build-gdal -j16 
```

commands.

Regarding GDAL CMake support see [Using GDAL in CMake projects](https://gdal.org/development/cmake.html) page.
