# Solid Game Engine

Licensed under Apache License 2.0

Still under construction !!

MinGW-w64 from Git for Windows recommended for the Windows platform.

> mingw32-make.exe all

Macos 10.13 is the last supported release at the moment, since OpenGL was removed from newer releases of macos.

> make all

QtCreator qbs also supported and recommended. Simply open the Qt .qbs file instead of using the Makefiles.

## Symbolically link the following directories from Huldra Dropbox

On macos and linux, solid has dependencies such as SDL. Pre-compiled shared objects and tools made specifially for solid are available. You may need to download the following files to get the most out of solid.

lib

tools

Feel free to get these files from here: https://drive.google.com/file/d/1sp1VJ-c7ZjEn3SB5Z8dM9TdFxOhOPwv1/view?usp=sharing

This can be done on both Windows and unix systems.

For Unix:

> ln -s ~/Huldra\ Dropbox/solid/lib lib

> ln -s ~/Huldra\ Dropbox/solid/tools tools

For Windows:

> mklink /D lib "../Huldra Dropbox"/solid/lib

> mklink /D tools "../Huldra Dropbox"/solid/tools

## Valkeryie support

On arch linux you need qt4 to run valkeryie in tools/linux (64-bit)

Downlioad qt4 from AUR: https://aur.archlinux.org/packages/qt4

> tar xfv qt4
> cd qt4
> makepkg
> pacman -U qt4.pkg.tar.szt

Remember to install valgrind

> pacman -S valgrind

You may also want imagemagick

> pacman -S imagemagick
