# Stoff2D
Stoff2D is a game engine/library written in C for windows. It is intended to be
built as a static library.

## Features and Usage
Stoff2D is a modular collection of libraries. Each submodule is standalone and does
not depend on the others.

Visit each s2d_X header to see what is available, and checkout the example projects
in examples to see how stoff2d can be used to create a game.

To control lower level details like max entites/particles or even quads per draw
call, have a look at stoff2d/stoff2d_core/settings.h, here you can also set the 
resource folder locations.

stoff2d_ecs is designed to be extended. To add your own components simply edit 
components.h and recompile. For this reason I recommend including the source code 
for stoff2d_ecs as apart of your project (it is quite small).

stoff2d_core:
- windowing and input
- 2D quad renderer (coloured/textured)
- sprite layering
- particle system
- animations (see README.ani)

stoff2d_ecs:
- entity component system

## Build
Stoff2D uses CMake, so to build from the command line.
```
> git clone https://github.com/Stoff-Studios/Stoff2D
> cd Stoff2D
> mkdir build
> cd build
> cmake ..
> cmake --build .
```
All binaries will be in path/to/build/Debug

## Example Projects
To run the example projects, from the root directory run
```
> build/Debug/example_project
```
For now there is only shooter, more coming soon!
