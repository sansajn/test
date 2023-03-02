The directory serves as safe harbor for std algorithm samples ...

Build all samples with

```bash
cmake -B build-algo -S algo
cmake --build build-algo -j16
```

commands from outside of the (`algo`) directory.

Specific target can be build with

```bash
cmake --build build-algo --target TARGET`
```

build command.

Run specific sample `SAMPLE` with

```bash
build-algo/SAMPLE
```

command.

