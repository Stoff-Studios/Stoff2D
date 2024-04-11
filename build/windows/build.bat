@ECHO off


REM Build script for a stoff2d project

PUSHD "..\..\"

REM Get list of all .c files
SetLocal EnableDelayedExpansion
SET srcFiles=
FOR /R %%f in (*.c) do (
    SET srcFiles=!srcFiles! %%f
)

ECHO creating bin directory...
MKDIR bin

SET projName=Stoff2D
SET includes=/I stoff2d/include /I stoff2d/vendor/include /I project/include
SET compileFlags=/Febin/%projName%.exe /EHsc /MD %includes%
SET linkerFlags=/link /LIBPATH:stoff2d/vendor/lib /NODEFAULTLIB:MSVCRTD
SET libs=glfw3.lib opengl32.lib user32.lib gdi32.lib shell32.lib ws2_32.lib

cl %compileFlags% %srcFiles% %includeFlags% %libs% %linkerFlags%

DEL *.obj

EndLocal

POPD
