#!/bin/sh

git submodule init
git submodule update

# I don't want to go to the trouble of distributing a modified version of the library
# and I have absolutely no idea what I'm doing lmao

sed -i 's/CPP = gcc/CPP = clang++/' ./libfixmath/libfixmath/Makefile
sed -i 's/CC = gcc/CC = clang/' ./libfixmath/libfixmath/Makefile
sed -i 's/INC =/INC = -DFIXMATH_NO_64BIT -DFIXMATH_FAST_SIN -DFIXMATH_NO_ROUNDING -DFIXMATH_NO_CTYPE/' ./libfixmath/libfixmath/Makefile

cd ./libfixmath/libfixmath && make && cd ../..

mkdir ./build

make all