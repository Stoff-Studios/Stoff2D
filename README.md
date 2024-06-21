# Stoff2D
Stoff2D is a cross platform C library for making games!!

## Features and Usage
Stoff2D is a modular collection of libraries. Each submodule can be used in
isolation.

Visit each stoff2d header to see what is available, and checkout the example projects
in examples to see how stoff2d can be used to create a game. 

To control lower level details like max entites/particles or even quads per draw
call, have a look at include/settings.h and include/defines.h, here you can also 
set the resource folder locations for your project.

stoff2d_ecs is designed to be extended. To add your own components simply edit 
components.h and recompile. For this reason I recommend including Stoff2D as a
submodule.

Do not remove the white texture in res/textures, this is used by the renderer to
draw coloured quads. You can move the png as long as white.png is in the 
S2D_TEXTURE_FOLDER location (see include/settings.h)

stoff2d_core:
- windowing and input
- coloured/textured quad rendering 
- sprite layering
- particle system
- animation (see res/animations/README.ani)
- text rendering

stoff2d_ecs:
- entity component system (see component.h)

## Dependencies
- glfw

Linux users will need some glfw dependencies, check out the
Installing Dependencies section here if you get errors.
https://www.glfw.org/docs/latest/compile.html

## Build
Stoff2D is built using CMake. Make sure to recursively clone.
```
> git clone https://github.com/Stoff-Studios/Stoff2D --recurse-submodules
> cd Stoff2D
> mkdir build
> cd build
> cmake ..
> cmake --build .
```

## Example Projects
To build the example projects, turn on the BUILD_EXAMPLES option when calling 
cmake. Just add -DBUILD_EXAMPLES=ON before the source directory.
```
> cmake -DBUILD_EXAMPLES=ON ..
> cmake --build .
```
To run the example games, make sure the binary is called from the root
(checkout settings.h to change resource locations).

Current example games (more to come)
#### Shooter
*controls*
- move           -> wasd
- shoot          -> arrow-keys
- pause          -> p
- resume         -> r
- show hitboxes  -> h
- zoom in        -> i
- zoom out       -> o
- fullscreen     -> f
- windowed       -> v
- quit           -> q

## Future Plans
- stoff2d_audio: (NEW MODULE) 
- stoff2d_core: remove white texture and just create one on startup.
