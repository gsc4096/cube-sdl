# SDL Cube
#### Video Demo: https://youtu.be/WB45vRMXsk8
#### Description: 

The present repository contains a graphical program that draws a rotating cube, serving as the final project for [CS50](https://www.edx.org/cs50). 

Per course requirements, it includes:
- C++ as it's programming language;
- SDL for rendering;
- CMake for building;
- g++ for compiling;
- Neovim for editing;
- clangd as LSP;
- clang-format for formatting;
- clang-tidy for linting.

Furthermore, the key files and their functions are:
- cmake/* - auxiliary CMake files to find SDL libraries
- include/gmath.h - header for custom math functions
- src/gmath.cc - corresponding code for custom math functions
- src/main.cc - game loop and event handling
- CMakeLists.txt - recipe for building the project
- .clang-format - definition of formatting standards

To handle rotation, a set of hard-coded vertices are transformed via [rotation matrix](https://en.wikipedia.org/wiki/Rotation_matrix)'s formula, applied around the yaw axis.

The idea I came up for handling the perspective is simple, but as I've learnt throughout the project, rather unorthodox: vertices stored in an array are iterated, and their angles from the observer are calculated. Such angles are compared to the FOV of the camera, which then determines their position in normalized device coordinates (NDC). After denormalization, the data is ready for the rendering stage.

(I made an [interactive demo in Desmos](https://www.desmos.com/calculator/pge0socfbx), for the sake of illustration.)

An array storing vertex pairs is used to determine the lines' ends, which are rendered alongside circles at the vertices. The loop finishes by adjusting the frame rate to avoid tearing.

The cube's movement can be controlled with left and right keys. A clamped velocity vector is used to smoothly accelerate when a key is pressed, and decelerate when not.

Nothing was AI-generated, nor AI-assisted.

#### Some personal context

Something I've wanted to experiment with for a long time is [computer graphics](https://en.wikipedia.org/wiki/Computer_graphics). Due to shifting interests, however, I never *truly* delved into the ideas within it. This project seeks to change that, and perhaps, be a kickstarter for more ambitious ones.

I settled for drawing a wireframe cube on a screen, since it seemed doable considering previous experience with gamedev and some very basic linear algebra. I chose C++ as the coding language for two reasons: firstly, I used it in the past for coding competitions, and secondly, SDL seemed to be used alongside it.

SFML was considered, since it's syntax is closer to C++ and farther from C. Nonetheless, SDL is more widespread in the graphics industry, potentially offering more support in case I needed it.

Using CMake was purely incidental, as I needed `compile_commands.json` for clangd/clang-tidy to work properly.

#### Building instructions

```shell
cmake -S . -B build
cd build && make && ./cube
```

Make sure to have CMake and SDL libraries installed on your system.
