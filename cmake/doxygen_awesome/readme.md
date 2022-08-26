This directory contains CMake doxygen (document generation) with awesome template integration sample.

Requires *doxygen*, *graphviz* packages to be installed with

```bash
sudo apt install doxygen graphviz
```

command.

Build with

```bash
cmake -B build-doxygen_awesome -S doxygen_awesome
cmake --build build-doxygen_awesome -j16 -t doxygen
```

commands to generate documentation.
