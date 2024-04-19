# About

SDL samples.

Install SDL2 and other dependencies with

```bash
sudo apt install libsdl2-dev libsdl2-mixer-dev libgles-dev libglm-dev
```

command.

Run

```bash
scons [target]
```

command to build samples (e.g. `scons hello` to build `hello.cpp` sample).


**ToDo**:
- add CMake project (so we can define library versions)
- full screen sample
- mouse events
- keyboard events
- imgui integration