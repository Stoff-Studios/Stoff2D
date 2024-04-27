@ECHO off

REM default run debug
IF "%~1"=="release" GOTO release
IF "%~1"=="" GOTO debug

:release
bin\Release\Shooter.exe
EXIT

:debug
bin\Debug\Shooter.exe
EXIT
