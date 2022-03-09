DEFS_COMMON = -Wall -Wextra -O3 -std=c++20 -DFIXMATH_NO_64BIT -DFIXMATH_FAST_SIN -DFIXMATH_NO_ROUNDING -DFIXMATH_NO_CTYPE
DEFS_HOSTED = $(DEFS_COMMON) -lSDL2 -lSDL2_ttf -DHOSTED
DEFS_BARE = $(DEFS_COMMON) -nostdlib -target wasm32

all: build/Minecraft4k.wat build/Minecraft4k

build/Minecraft4k: src/Minecraft4k.cpp src/sdl_wrapper.hpp
	clang++ src/Minecraft4k.cpp -Ilibfixmath/libfixmath -I. -o build/Minecraft4k $(DEFS_HOSTED) -Llibfixmath/libfixmath -lfixmath

build/Minecraft4k.o: src/Minecraft4k.cpp src/sdl_wrapper.hpp
	clang++ src/Minecraft4k.cpp -Ilibfixmath/libfixmath -I. -o build/Minecraft4k.o -c $(DEFS_BARE)

build/Minecraft4k.wasm: build/Minecraft4k.o build/libfixmath.o
	wasm-ld $^ -o $@ --allow-undefined --lto-O3

build/Minecraft4k.wat: build/Minecraft4k.wasm
	wasm2wat $^ -o $@


SRC = libfixmath/libfixmath

PROJECT = libfixmath
LIB =
INC =

#Compiler settings
CPP = clang++
CC = clang
LD = wasm-ld
AR = ar
CPP_FLAGS = -O3 $(INC) -Wall -Wextra -c -DFIXMATH_NO_64BIT -DFIXMATH_FAST_SIN -DFIXMATH_NO_ROUNDING -DFIXMATH_NO_CTYPE -target wasm32 -nostdlib -O3
CC_FLAGS  = -O3 $(INC) -Wall -Wextra -c -DFIXMATH_NO_64BIT -DFIXMATH_FAST_SIN -DFIXMATH_NO_ROUNDING -DFIXMATH_NO_CTYPE -target wasm32 -nostdlib -O3

# Find all source files
SRC_C   = $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
OBJ     = $(patsubst libfixmath/libfixmath/%.c, build/%.o, $(SRC_C))

$(info $$SRC_C is [${SRC_C}])
$(info $$OBJ   is [${OBJ}])

build/libfixmath.o: $(OBJ)
	wasm-ld -o build/libfixmath.o $(OBJ) --no-entry -relocatable --lto-O3

$(OBJ) : build/%.o : libfixmath/libfixmath/%.c
	clang $(CC_FLAGS) -o $@ $<