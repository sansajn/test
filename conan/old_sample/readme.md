# About

The directory contains *Conan* package manager sample.

> **note**: this is no longer buildable under *Kubuntu 22.04 LTS* due to *Conan* changes at 7.8.2024

> **dependencies**: Before we can continue we need to install Conan and setup default profile.

# Build

Build with

```bash
mkdir build && cd build
conan install ..
cmake ..
camke --build .
```

commands from inside the project directory.


In case the `conan install` ends with *ERROR: Missing prebuilt package for xyz* error e.g.

```
ERROR: Missing prebuilt package for 'bzip2/1.0.8', 'expat/2.5.0', 'openssl/1.1.1t', 'pcre/8.45', 'poco/1.9.4',  
'sqlite3/3.40.1', 'zlib/1.2.13'. You can try:  
   - List all available packages using 'conan list "bzip2/1.0.8:*" -r=remote'  
   - Explain missing binaries: replace 'conan install ...' with 'conan graph explain ...'  
   - Try to build locally from sources using the '--build=missing' argument
```

run `conan install` with `--build=missing` argument tihs way

```bash
conan install --build=missing ..
```

# Run

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

> **question**: Default *Conan* behaviour?
