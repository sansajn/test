This directory contains doxygen integration sample.

Requires *doxygen*, *graphviz* packages to be installed with

```bash
sudo apt install doxygen graphviz
```

command.

Build with

```bash
cmake -B build-doxygen -S doxygen
cmake --build build-doxygen -j16 -t doxygen 
```

commands to generate documentation.
