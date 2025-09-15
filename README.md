# Solid Game Engine

Licensed under Apache License 2.0

MinGW-w64 from Git for Windows recommended for the Windows platform.

> mingw32-make.exe all

Macos 10.13 is the last supported release at the moment, since OpenGL was removed from newer releases of macos.

> make all

QtCreator qbs also supported and recommended. Simply open the Qt .qbs file instead of using the Makefiles.

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
