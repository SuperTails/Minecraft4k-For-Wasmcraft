# Minecraft4k for Wasmcraft

This is the source code for the "Minecraft in Minecraft" demo shown off for [Wasmcraft 2](https://github.com/SuperTails/wasmcraft2).
It is a heavily modified C++ rewrite of [Minecraft4k-Reversed](https://github.com/RealTheSunCat/Minecraft4k-Reversed).

## Building

You will need some dependencies first. Using apt:

`$ sudo apt install clang lld wabt libsdl2-dev`

Or using pacman:

`$ sudo pacman -S clang lld wabt sdl2`

Copy the `mcinterface.h` file from Wasmcraft 2 to the root of this directory. Then, simply run `sh build.sh`.
This will make 3 main files.

* `build/Minecraft4k` is a normal binary that uses an SDL2 shim to show what the program should look like without actually running it in Minecraft
* `build/Minecraft4k.wasm` is a WebAssembly file. This is the file you will provide to Wasmcraft.
* `build/Minecraft4k.wat` is a text version of the WebAssembly output, occasionally useful for debugging or hand optimization.

Note that there is no mouse movement support yet, though breaking blocks works.