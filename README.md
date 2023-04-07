Symbolically link the following directories from Huldra Dropbox

lib
standarddata 
tools

This can be done on both Windows and unix systems.

For Unix:

> ln -s ~/Huldra\ Dropbox/solid/lib lib
> ln -s ~/Huldra\ Dropbox/solid/standarddata standarddata
> ln -s ~/Huldra\ Dropbox/solid/tools tools

For Windows:

> mklink /D lib "../Huldra Dropbox"/solid/lib
> mklink /D standarddata "../Huldra Dropbox"/solid/standarddata
> mklink /D tools "../Huldra Dropbox"/solid/tools
