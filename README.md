# Stoff2D
Stoff2D is a cross platform C library for making games!!

## Features and Usage
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
Linux users will need some glfw dependencies since glfw is built as a submodule, 
check out the "Installing Dependencies" section here if you get errors.
https://www.glfw.org/docs/latest/compile.html

## Build
Stoff2D is built using CMake. Make sure to recursively clone.
```
> git clone https://github.com/Stoff-Studios/Stoff2D --recurse-submodules
> cd Stoff2D
> mkdir build
> cd build
> cmake -DBUILD_EXAMPLES=ON .. 
> cmake --build .
```
note the flag -DBUILD_EXAMPLES=ON, omit this when building as apart of a larger
project if you don't care about the test programs.

## Example Projects
To run the examples, call the binary built from the root directory. The res 
folder is expected to be in the same directory that the binary was called from 
by default. This can be changed in settings.h
```
> build/examples/shooter/Debug/shooter.exe
> build/examples/test/Debug/test.exe
```

Current examples
#### Shooter
vampire survivors type playbox
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
#### test
testbed used mainly for developing new features. Lots of particles.

## Future Plans
- stoff2d_audio: (NEW MODULE) 
