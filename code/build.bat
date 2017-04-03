@echo off

set CommonCompilerFlags=-MTd -nologo -Gm- -GR- -EHa- -Od -Oi -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DINTERNAL_BUILD=1 -DSLOW_BUILD=1 -D_CRT_SECURE_NO_WARNINGS -FC -Z7 -I ../neongod/include
set CommonLinkerFlags=-opt:ref user32.lib gdi32.lib opengl32.lib SDL2main.lib SDL2.lib glew32.lib freetypeMTd.lib
REM freetypeMTd.lib

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build

REM 32-bit build
REM cl  %CommonCompilerFlags%..\neongod\code\neon_sdl.cpp /link -subsystem:windows,5.1 %CommonLinkerFlags%

REM 64-bit build
cl  %CommonCompilerFlags% ..\neongod\code\neon_sdl.cpp /link /libpath:../neongod/lib/x64 %CommonLinkerFlags%
popd
