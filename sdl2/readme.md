# About

SDL samples.

Install SDL2 and other dependencies with

```bash
sudo apt install libsdl2-dev libsdl2-mixer-dev libgles-dev libglm-dev libfmt-dev
```

command.

## SCons

Run

```bash
scons [target]
```

command to build samples (e.g. `scons hello` to build `hello.cpp` sample).


## (not working) CMake

Run

```bash
cmake -B build-sdl2 -S sdl2
cmake --build build-sdl2 -j16
```

commands.




**ToDo**:
- full screen sample
- mouse events
- keyboard events
- imgui integration
- UBO sample (I need more complex sample for that)
- add CMake project (so we can define library versions, not support for glesv2 library)
