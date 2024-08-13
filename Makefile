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
INCLUDEP:=-Iinclude -Isrc -Isrc/engine
LIBP:=-Llib
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
	@for dir in $(DIRS); do \
		mkdir -p bin/$$dir; \
	done

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
	$(CC) -g -c $(DEFINES) $(INCLUDEP) $^ -o ./bin/$@