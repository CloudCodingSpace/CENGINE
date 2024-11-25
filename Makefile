# Globals
define recurfind
	$(wildcard $(1)/$(2)) $(foreach dir,$(wildcard $(1)/*),$(call recurfind,$(dir),$(2)))
endef

TARGET=engine
CC=gcc
DEFINES:=-D_DEBUG -D_WIN32
INCLUDEP:=-Iinclude -Isrc -Isrc/engine -I$(VULKAN_SDK)/Include
LIBP:=-Llib -L$(VULKAN_SDK)/Lib
LINKFLAGS:=-lglfw3dll -lvulkan-1

SRCS:=$(call recurfind,src,*.c)
VERTS:=$(call recurfind,shaders,*.vert)
VERTSPV:=$(patsubst %.vert,%.vert.spv,$(VERTS))
FRAGS:=$(call recurfind,shaders,*.frag)
FRAGSPV:=$(patsubst %.frag,%.frag.spv,$(FRAGS))
OBJS=$(patsubst %.c,%.o,$(SRCS))
DIRS:=$(dir $(SRCS))
OBJS_BIN:=$(patsubst %,bin/%,$(OBJS))

# Recipes
.SILENT:
.PHONY: all

all: createdirs build

createdirs:
ifeq ($(OS),Windows_NT)
	@for %%D in ($(DIRS)) do ( \
		if not exist "bin\%%~D" mkdir "bin\%%~D" )
else
	@mkdir -p $(patsubst %/,bin/%,$(DIRS))
endif

build: build_shaders $(OBJS) $(VERTSPV) $(FRAGSPV)
	echo "Linking the final binary..."
	$(CC) $(LIBP) $(OBJS_BIN) $(LINKFLAGS) -o $(TARGET)

build_shaders:
	echo "Compiling the vertex shader code..."
	glslc $(VERTS) -o $(VERTSPV)
	echo "Compiling the fragment shader code..."
	glslc $(FRAGS) -o $(FRAGSPV)

run:
	echo "Running $(TARGET)..."
	./$(TARGET)

clean:
	echo "Cleaning up..."
ifeq ($(OS),Windows_NT)
	@if exist bin (rmdir /s /q bin)
	@if exist $(TARGET).exe del /q /f $(TARGET).exe
	@for %%F in (*.vert.spv *.frag.spv) do (if exist %%F del /q /f %%F)
else
	rm -rf bin $(TARGET) *.vert.spv *.frag.spv
endif

# Rules
%.o: %.c
	echo "Compiling bin/$@ from source file $^..."
	$(CC) -g --std=c17 -c $(DEFINES) $(INCLUDEP) $^ -o ./bin/$@