# Globals
define recurfind
	$(wildcard $(1)/$(2)) $(foreach dir,$(wildcard $(1)/*),$(call recurfind,$(dir),$(2)))
endef

define find_subdirs
  $(wildcard $(1)*/) $(foreach dir,$(wildcard $(1)*/),$(call find_subdirs,$(dir)))
endef

TARGET=engine.exe
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
	@for %%D in ($(DIRS)) do ( \
		if not exist bin/%%D ( \
			mkdir bin/%%D \
		) \
	)

build: build_shaders $(OBJS) $(VERTSPV) $(FRAGSPV)
	echo "Linking the final binary..."
	$(CC) $(LIBP) $(OBJS_BIN) $(LINKFLAGS) -o $(TARGET)

build_shaders:
	echo "Compiling the vertex shader code..."
	glslc $(VERTS) -o $(VERTSPV)
	echo "Compiling the fragment shader code..."
	glslc $(FRAGS) -o $(FRAGSPV)

run:
	echo "Running the binary..."
	./engine

# Rules
%.o: %.c
	echo "Compiling bin/$@ from source file $^..."
	$(CC) -g --std=c17 -c $(DEFINES) $(INCLUDEP) $^ -o ./bin/$@