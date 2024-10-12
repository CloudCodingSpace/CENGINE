@echo off
SetLocal EnableDelayedExpansion

call build-shaders

set srcs=
for /R %%f in (*.c) do (
	set srcs=!srcs! %%f
)

set includep=-Iinclude -Isrc -Isrc/engine -I%VULKAN_SDK%/Include
set libp=-Llib -L%VULKAN_SDK%/Lib
set defines=-D_DEBUG -D_WIN32
set linkerflags=-lvulkan-1 -lglfw3dll

gcc -g --std=c23 -Wall %defines% %includep% %libp% %srcs% %linkerflags% -o engine
