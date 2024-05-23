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
Stoff2D compiles for Windows/Linux/Mac using CMake.
To build from the command line.
```
> git clone https://github.com/Stoff-Studios/Stoff2D
> cd Stoff2D
> mkdir build
> cd build
> cmake ..
> cmake --build .
```
Binaries will be placed in build/Debug or build

## Example Projects
To run the example projects, make sure the binary is called from the same 
directory as the res folder.
Current example games (more to come)
- shooter

## Project Configuration
If you want to change some core settings, simply have a look at 
stoff2d_core/settings.h, here you can change specific folder locations as well
as max animaiton frames and entities. It is highly recommmended to tailor these
to your projects needs. 

Make sure the white texture is not removed since it is used for coloured quad
rendering.
