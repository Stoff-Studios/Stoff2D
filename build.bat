@ECHO off

IF "%~1"=="clean" GOTO clean 
IF "%~1"=="" GOTO build

:clean
DEL **.sln
DEL **.vcxproj
DEL **.vcxproj.user
DEL **.vcxproj.filters
@RD /S /Q bin
@RD /S /Q bin-int 
@RD /S /Q .vs
EXIT

:build
call vendor\premake5.exe vs2022
EXIT
