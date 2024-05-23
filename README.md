# Stoff2D
Stoff2D is a cross platform C library for making games!!!

## Features and Usage
Stoff2D is a modular collection of libraries. Each submodule is standalone 
and does not depend on the others.

Visit each s2d_X header to see what is available, and checkout the example projects
in examples to see how stoff2d can be used to create a game. Ofcourse have a peep
through everything as you please, but only features contained in headers prepended 
with "s2d" are expected to be called by the user.

To control lower level details like max entites/particles or even quads per draw
call, have a look at include/settings.h and include/defines.h, here you can also 
set the resource folder locations.

stoff2d_ecs is designed to be extended. To add your own components simply edit 
components.h and recompile. For this reason I recommend including the source code 
for stoff2d_ecs as apart of your project (it is quite small).

do NOT remove the white texture in res/textures, this is used by the renderer to
draw coloured quads. You can move the png just make sure there is a png called
"white.png" in the S2D_TEXTURE_FOLDER location (see include/settings.h)

stoff2d_core:
- windowing and input
- 2D quad renderer (coloured/textured)
- sprite layering
- particle system
- animations (see README.ani)

stoff2d_ecs:
- extendable entity component system (see component.h)

## Dependencies
stoff2d_core uses glfw, so throw this in your libs directory alongside it.

## Build
Stoff2D is built using CMake, from the command line.
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
