# AGENTS.md

## Agent framework

The coding agent harness is called pi

A model has access to four tools only: read, write, edit, and bash
 
## Project Overview

This is a C++ game project.

- Language: C++
- Build system: Make (Makefile)
- Output: Native executable

The project is intended to be compiled and run locally.

## Build Instructions

To compile the project, run:

> make

To clean build artifacts:

> make clean

## Run Instructions

After building, run the executable:

> make run

## Code Guidelines
- Use modern C++ (C++17 or newer if possible)
- Prefer clear, readable code over clever optimizations
- Keep functions small and focused
- Use meaningful variable and function names
- Avoid unnecessary dependencies

## Project Structure

./source       -> Source files (.cpp and .h)
./data         -> Game assets (textures, audio, etc.)
./build        -> Build output
./bin          -> Binary files output
./intermediate -> Intermediate files from the Asset Conditioning Pipeline
Makefile       -> Build configuration

## Agent Instructions

When modifying this project:

- Always ensure the project compiles with make
- Do not introduce build systems other than Make
- Do not break existing functionality unless explicitly instructed
- Keep changes minimal and well-scoped
- Maintain consistency with existing code style
- If adding files, update the Makefile accordingly
- Prefer simple and robust solutions over complex ones

## Game-Specific Notes
- This is a game project, so performance and responsiveness matter
- Avoid blocking the main loop
- A custom game engine called solid is used
- Keep rendering and logic cleanly separated where possible
- Use the renderer, audio, filesystem, input, physics object abstractions from solid
- Be cautious when modifying core gameplay systems
- Testing & Validation

## After making changes:

- Build with make
- Run the game
- Ensure no crashes or major regressions occur

## Constraints
- No external frameworks unless explicitly requested
- No breaking changes to build pipeline
- Must remain compatible with standard C++ toolchains (e.g., g++, clang)

## Summary

- This is a simple C++ game project using Make.
- Keep it compilable, stable, and simple.
