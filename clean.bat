@echo off
SetLocal EnableDelayedExpansion

set srcs=
for /R %%f in (*.o) do (
	set srcs=!srcs! %%f
)

for /R %%f in (*.d) do (
	set srcs=!srcs! %%f
)

for /R %%f in (*.spv) do (
	set srcs=!srcs! %%f
)

del %srcs%