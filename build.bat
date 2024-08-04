@echo off
SetLocal EnableDelayedExpansion

call build-shaders

set srcs=
for /R %%f in (*.c) do (
	set srcs=!srcs! %%f
)

set includep=-Iinclude -Isrc -Isrc/engine
set libp=-Llib
set defines=-D_WIN32
set linkerflags=-lvulkan-1 -lglfw3dll

gcc -g --std=c23 -Wall %defines% %includep% %libp% %srcs% %linkerflags% -o engine
