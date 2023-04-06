# About

The directory contains conan package manager sample.

Build with

```bash
mkdir build && cd build
conan install ..
cmake ..
camke --build .
```

from inside the project directory.

Run sample with

```console
$ ./bin/hello 
c3fcd3d76192e4007dfb496cca67e13b
```

see [Getting Started](https://docs.conan.io/1/getting_started.html) article for mere details ...

Library dependencies are downloaded into `~/.conan` directory (see `data/`).

It looks like instead of dynamic libraries, static ones are used

```console
$ ldd ./bin/hello 
	linux-vdso.so.1 (0x00007ffe0b9f6000)
	libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fb040200000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fb040530000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fb03fe00000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fb040447000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fb040591000)
```

> **question**: Default conan behaviour?
