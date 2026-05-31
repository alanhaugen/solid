# Solid Game Engine

Licensed under Apache License 2.0

MinGW-w64 from Git for Windows recommended for the Windows platform.

> mingw32-make.exe all

Macos 10.13 supports OpenGL, Vulkan via MoltenVK on newer releases.

QtCreator qbs also supported and recommended. Simply open the Qt .qbs file instead of using the Makefiles.

# Build Instructions

To compile the project, run:

> make

To clean build artifacts:

> make clean

Run Instructions
After building, run the executable:

> make run

**Note:** An example application can be found in examples/templates/app

> make all

# Project Overview

This is a C++ game project.

Language: C++
Build system: Make (Makefile)
Output: Native executable
The project is intended to be compiled and run locally.

# Code Guidelines

Use C++
Prefer clear, readable code over clever optimizations
Keep functions small and focused
Use meaningful variable and function names
Avoid unnecessary dependencies

# Project Structure
./source -> Source files (.cpp and .h) ./data -> Game assets (textures, audio, etc.)
./build -> Build output
./bin -> Binary files output
./intermediate -> Intermediate files from the Asset Conditioning Pipeline Makefile -> Build configuration
