# CellularAutomata
Various types of 2d cellular automaton implemented in C++ and OpenGL

## Features
- **Multiple Cellular Automata Types**:
  - Conway's Game of Life
  - Brian's Brain
  - Day and Night
- **Interactive Controls**:
  - Left click to place living cells
  - Right click to remove cells
  - Mouse scroll to adjust brush size
  - ESC to exit
- **Display Modes**:
  - Press **F11** to toggle between fullscreen and windowed modes
  - Window can be resized in windowed mode
  - OpenGL framebuffer automatically adjusts to window resolution

## Building

### Prerequisites
- C++ compiler with C++11 support
- OpenGL libraries
- GLFW3
- GLEW
- GLM

### On Ubuntu/Debian:
```bash
sudo apt install libglew-dev libglfw3-dev libglm-dev
```

### Compilation:
```bash
make
```

## Running
```bash
./cellular_automata
```

Select a cellular automaton type from the menu:
1. Conway's Game of Life
2. Brian's Brain
3. Day and Night

## Controls
- **Left Mouse Button**: Place living cells
- **Right Mouse Button**: Remove cells
- **Mouse Scroll**: Adjust brush size
- **F11**: Toggle fullscreen/windowed mode
- **ESC**: Exit application

