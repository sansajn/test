Directory contains following Boost.Assign

- `fill_vector.cpp`: Insert values at the end of vector with Boost.Assign `operator+=()` overload.
- `insert_map.cpp`: Insert into map with Boost.Assign `operator()()` and `insert()` sample.

samples.

Build with

```bash
cmake -B build-assign -S assign
cmake --build build-assign -j16
```

> **tip**: to trigger only specific target use `--target TARGET` option

commands.
