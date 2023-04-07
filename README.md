# Solid Game Engine

Licensed under Apache License 2.0

MinGW-w64 from Git for Windows recommended for the Windows platform.

Macos 10.13 is the last supported release at the moment, since OpenGL was removed from newer releases of macos.

> make all

QtCreator qbs also supported and recommended. Simply open the Qt .qbs file instead of using the Makefiles.

## Symbolically link the following directories from Huldra Dropbox

On macos and linux, solid has dependencies such as SDL. Pre-compiled shared objects and tools made specifially for solid are available. You may need to download the following files to get the most out of solid.

lib

tools

Feel free to get these files from here: https://www.dropbox.com/sh/joej4i4hpsey87x/AABiXIso5Ns8vg-on9rm6A3oa?dl=0

This can be done on both Windows and unix systems.

For Unix:

> ln -s ~/Huldra\ Dropbox/solid/lib lib

> ln -s ~/Huldra\ Dropbox/solid/tools tools

For Windows:

> mklink /D lib "../Huldra Dropbox"/solid/lib

> mklink /D tools "../Huldra Dropbox"/solid/tools
