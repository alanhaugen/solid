/*! \mainpage Solid Index Page
 *
 *    Copyright 2023 Huldra Technology
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 * version 1, 2023
 *
 * Solid supports textures in common image formats by including ImageMagick by ImageMagick Studio LLC
 *
 * NOTICE: Products which use this software must include the third party licenses (\ref third_party) in the product documentation.
 *
 * This doc is written for people who want to use Solid to develop software.
 *
 * \section acknowledgements Acknowledgements
 *
 * Thanks to:
 * * Erik Haugen for making art, design and audio
 * * Torbjørn Haugen for coding and coding ideas
 *
 * \section source Getting the source
 *
 * git is required, see Installing dependencies
 *
 *  > git clone -\-recursive ssh://user@10.0.0.141:29418/SolidCore.git
 *
 * *NOTE: If you have already done a git clone and forgot to include -\-recursive, and you would like to have the source-code
 * of the third party software, which includes bison and the texture encoders, you can type the following:*
 *
 * > git submodule update --init \n
 * > git submodule update
 *
 * *NOTE: Gerrit does not support the current OpenSSH encryption algorithms, so add this to ~/.ssh/config:*
 *
 * <code>
 * > Host 10.0.0.141 \n
 * > [TAB] \span KexAlgorithms +diffie-hellman-group1-sha1
 * </code>
 *
 * \section dependencies Installing dependencies
 *
 * \subsection dependencies_arch Arch Linux
 *
 * Build essentials and git:
 *  > pacman -S base-devel git
 *
 * For Windows cross-compile support:
 *  > pacman -S mingw-w64-cross-gcc
 *
 * For Android support:
 *  > pacman -S android-ndk android-tools
 *
 * \subsection dependencies_linux Debian based Linux
 *
 * You will need a lot of stuff:
 *
 * > apt-get install build-essential git cmake cmake-curses-gui xorg-dev libgl1-mesa-dev libglu-dev
 *
 * > apt-get install libpng-dev libcurl4-openssl-dev libfreetype6-dev libjpeg-dev libvorbis-dev libopenal-dev libphysfs-dev libgtk2.0-dev libasound-dev libpulse-dev libflac-dev libdumb1-dev
 *
 * > apt-get install bison flex
 *
 * You probably need all that stuff on any linux distro, some distros have most of the packages pre-installed.
 *
 * \subsection dependencies_macos macOS
 *
 * You will find the terminal emulator in /Applications/Utilities/Terminal.app
 *
 * XCode Command Line Build Tools:
 *  > git clone -\-recursive ssh://user@10.0.0.141:29418/SolidCore.git \n
 *  > cd SolidCore \n
 *  > make
 *
 * Select Install in popup window
 *
 * \subsection dependencies_Windows Windows
 *
 * Install Git https://git-scm.com/download/win
 *
 * Install QtCreator Open-Source edition https://www.qt.io/download-open-source
 *
 * *Tip: During installation, click Deselect All and only select the newest MinGW compiler under tools*
 *
 * Add Git's /usr/bin to path so you get access to rm; run cmd.exe as Administrator
 * > setx /M PATH "%PATH%;C:\Program Files\Git\usr\bin"
 *
 * Open the QtCreator project SolidCore.creator
 *
 * \section build_toolchain Build Toolchain
 *
 * *GNU make* is required. Building should be as simple as running *make*, or pushing F5 or CMD+B if you are using QtCreator
 *
 * SolidCore will be compiled into libraries, which are not executable. The examples are executable, check them out.
 *
 * To run an executable, enter the project directory and open the QtCreator project file and push F5, then select the build in bin and set the working directory
 * to the same directory as the binary. If you are using the command line, enter the directory with the project's Makefile and run *make run*
 *
 * \subsection build_configuration Configuration
 *
 * Set supported modules as arguments to make. "make AUDIO=FmodAudio"
 *
 * VARIABLE                                       | DESCRIPTION                  | Windows          | Wii U     | Linux              | Android          | OS X
 * ------------                                   | -------------                | -------          | -----     | -----              | -------          | ----
 * PLATFORM=[mingw32\|linux\|cygwin\|ndk\|darwin] | Set build platform           | mingw32          | cygwin    | linux              | ndk              | darwin
 * AUDIO=[AllegroAudio\|FmodAudio]                | Sets audio renderer          | AllegroAudio     | sound2    | AllegroAudio       | AllegroAudio     | AllegroAudio
 * RENDERER=[GLESRenderer\|GLES2Renderer]         | Sets graphics renderer       | GLES2Renderer    | gx2       | es, GLES2Renderer  | es               | GLES2Renderer
 * PHYSICS=[bullet]                               | Sets body dynamics system    | bullet           | bullet    | bullet             | bullet           | bullet
 * SCRIPT=[angel\|poem]                           | Sets scripting system        | angel            | angel     | angel              | angel            | angel
 * FILESYSTEM=[std\|zlib]                         | Sets filesystem              | zlib             | zlib      | std, zlib          | zlib             | zlib
 *
 * \subsection build_examples Build examples:
 *
 * compile for your system:
 *  > make
 *
 * (cross-)compile for 32-bit Windows:
 *  > make PLATFORM=mingw32 PROCESSOR_ARCHITECTURE=x86
 *
 * compile debug build:
 *  > make debug
 *
 * (cross-)compile for 64-bit Windows with fmod audio:
 *  > make PLATFORM=mingw32 PROCESSOR_ARCHITECURE=x86_64 AUDIO=FmodAudio
 *
 * \subsection todo_build TODO:
 *  * Fix Android (NDK) support
 *
 * \subsection lexical_scanner_and_analyser Scanners and Parsers
 *
 * It is possible to add scanners and parsers to your projects. In your projects Makefile, add scanners and parsers to the variable SCANNERS and PARSERS.
 *
 * <code>
 * SCANNERS = \ \n
 *    calc.l
 *
 * PARSERS = \ \n
 *    calc.y
 * </code>
 *
 * The program will not be linked to the flex or bison library (-lfl) so you will need to provide your own main and yywrap, or add
 *
 * \%option noyywrap
 *
 * to the definition section in your scanner (the top of your .l file). Then you won't need to add yywrap (which is a function which should simply return 1; anyway).
 *
 * Here's an example of how the main function could look like, place it at the end of your .y
 *
 * <code>
 * main(int argc, char **argv) \n
 * { \n
 *     if(argc > 1)\n
 *     { \n
 *         if(!(yyin = fopen(argv[1], "r"))) \n
 *         { \n
 *             return (1); \n
 *         } \n
 *     } \n
 *     \n
 *     yyparse(); \n
 *     return 0; \n
 * }
 * </code>
 *
 * If you want to, you could also add yywrap.
 *
 * <code>
 * int yywrap() \n
 * { \n
 *     return 1; \n
 * }
 * </code>
 *
 * If you are going to have more than one scanner and parser, add
 *
 * \%option prefix="foo"
 *
 * to the definitions section in your .l file. Then add
 *
 * \%name-prefix "foo"
 *
 * to the definitions section of your .y file. Then you need to call the parser with fooparse(). You may also call the scanner with foolex(). It is normally yylex()
 *
 * Solid includes binaries for flex and bison for Windows, so you don't need to install them manually on the Windows platform
 *
 * \section application_configuration Further Configuration
 *
 * There are three ways of configuring an application
 *
 * \subsection command_options Command Line Options
 *
 * The following arguments are supported
 *
 * OPTION          | PARAMETER     	    	| EFFECT
 * ------------	   | ------------        	| -------------
 * --window-length | [wl]		            | Sets window width
 * --window-height | [wh]		            | Sets window height
 * --paused        |    			        | Start paused
 * --scene         | [sceneNumber]	        | Loads specified scene
 * --fullscreen    | [1\|0]		            | Sets fullscreen mode on/off
 * --monitor       | [monitor]		        | Which screen number to open on
 * --delta-time    | [deltaTime]	        | Controls how fast the simulation runs (default=0.01) (TODO: Change to --time-scale? Default to 1.0? Implement a semi-fixed deltatime?)
 * --test-run	   |		                | Makes application run for 20 seconds and quit
 *
 * *Example:*
 *  > ./game --paused --scene 2 --fullscreen 0
 *
 * The deltaTime is fixed, the simulation will run at 100 frames per second by default (waiting up to 10 milliseconds / 0.01 seconds per frame). When altering the time-scale, this
 * is what you alter. All services will be affected by the slowdown, so the simulation should be deterministic and no different at different speeds. The computer
 * must be fast enough to catch up to a desired time-scale for it to run at that time-scale. Note that this is not really ideal https://gafferongames.com/post/fix_your_timestep/
 *
 * \subsection text_configuration Text configuration files
 *
 * An application can be configured by text files, eg. project.json, <username>.json
 *
 * Place the configuration files next to the executable, or in <userhome>/<companyName>/<projectName>
 *
 * A configuration file may be incomplete, as default options will be filled in, also, a user
 * defined configuration file will override the project configuration file project.json
 *
 * \subsection environment_vars Environment Variables
 *
 * You may also define environment variables which will change default build behaviour,
 * which in turn changes the default configuration. See build_configuration for details.
 *
 * You could set these in QtCreator: Projects -> Build -> Build Environment (remember to set your configuration for both your executable project and for the SolidCore project)
 *
 * \section engine_sec Engine Overview
 *
 * \subsection actors_engine Actors
 *
 * \subsection scene_engine Scenes
 *
 * \subsection scripting_engine Scripting
 *
 * \subsection new_engine New project
 *
 * \subsection filesystem File system
 *
 * The file system is an abstraction for the ways the engine accesses data. You may load data from $(SOLIDCORE)/standarddata or from your projects data folder.
 *
 * When accessing data, the following scheme has been devised:
 *
 * host/path/to/resource.txt
 *
 * The following hosts are supported:
 *
 * * data
 * * standarddata
 *
 * Example:
 *
 * data/ui/menu.png
 *
 * \section structure_sec Structure
 *
 * Examples are found in examples, examples/templates are simple examples on how to make a new module or
 * application.
 *
 * DIRECTORY              | CONTENTS
 * ------------           | -------------
 * source/core            | consists of x-platform, platform and containers
 * source/core/x-platform | describes generic interconnected functions universal to all platforms
 * source/core/platform   | specifies platform specific code
 * source/core/containers | linked list, hash, array, etc. Sorry for not including stlport
 * source/core/components | standard components which can be added to "actors"
 * source/modules         | consists of audio, filesystem, physics, renderer and script.
 * source/modules/?/?.h   | each directory has a header which describes the implementation of the module
 * source/3rdparty        | consists of 3rdparty libraries used to build the framework
 * source/glm             | OpenGL Mathematics (GLM) header-only GLSL-like mathematics library
 * source/3rdparty/jsmn.h | JSMN header-only json parser library
 * lib                    | pre-compiled core, modules and 3rdparty libraries
 * examples               | examples showcasing various aspects of the framework
 * examples/templates     | example on how to make a basic application and module
 *
 * \subsection structure_platform Platform
 *
 * Application inherits PLATFORM, found in application.h
 *
 * PLATFORM inherits Locator which has the following services:
 *  * debug
 *  * renderer
 *  * audio
 *  * ai
 *  * physics (Body Dynamics)
 *  * chemistry (change/Reaction Dynamics)
 *  * cas
 *  * filesystem
 *  * scripts
 *
 * The application constructor should:
 *  * Invoke application base class constructor
 *  * * Seed a Random object
 *  * * Loads application configuration
 *  * Create a Debug object
 *  * Create an Audio module
 *  * Create a Physics module
 *  * Create a Scripts module
 *  * Create a Renderer module
 *
 * A Main method should:
 *  * Gather raw input and pass it to input handler with OnButton(const unsigned int button, bool isDown)
 *  * Call UpdateServices continuously
 *
 * \subsection structure Input
 *
 * Input consists of three layers
 *
 *  * Raw input gathering
 *  * Input mapping
 *  * High level handler
 *
 * The two first layers are platform specific, hence, their implementation should be done when implementing
 * a new application platform.
 *
 * input.OnButton(const unsigned int button, bool isDown) is called to pass raw input into the input handler.
 *
 * input.Define(const char *controlName, const unsigned int key, ...) is used for keymapping
 *
 * The data structure Input functions as the high-level handler, and can be used by an application to process
 * input. Example: if (Input.Held(Key.C)) { ... }
 *
 * Each platform should redefine Key in Application so it points to the correct keycodes. Ex: ... Key.C = 0x43; ...
 *
 * \subsection structure_components Components
 *
 * Actors consist of a gizmo (x, y, z position) and a list of components
 *
 * Components implement functionality, such as Sprite, Mesh, State, etc.
 *
 * \section buildbot Continuous Integration
 *
 * We use buildbot 0.9
 *
 * > buildbot start master
 *
 * On local machines
 * > buildbot-worker start mingw32worker
 * > buildbot-worker start docsworker
 * > buildbot-worker start cafeworker
 *
 * \section file_format File format support
 *
 * Supported image formats: *bzlib cairo freetype jng jp2 jpeg lcms lqr openexr pangocairo png ps rsvg tiff webp xml zlib*
 *
 * Supported 3D geometry formats: *fbx dae gltf glb blend 3ds ase obj ifc xgl zgl ply dxf lwo lws lxo stl x ac ms3d cob scn ogex x3d 3mf bvh csm xml irrmesh irr mdl md2 md3 pk3 mdc md5 smd vta ogex 3d b3d q3d q3s nff nff off raw ter mdl hmp ndo*
 *
 * Supported audio formats: *wav aiff pcm ogg flac mp2 mp3*
 *
 * \section profiling Profiling
 *
 * You could use RenderDoc (https://renderdoc.org/) for profiling of OpenGL and Vulkan renderers.
 *
 * \section documentation_sec Documentation
 *
 * We use doxygen
 *
 * Try to comment your code
 *
 * If you want your code descriptions to appear in the docs, use //!, they may span several lines
 *
 * To generate the documentation, you need doxygen:
 *  > make doc
 *
 * \section conventions_sec Coding Conventions
 *
 * Look at the following in application.h, which is where the doxygen main page is (you will need to open it in an editor)
 *
 * <code>
 * #include "core/application.h" \n
 * \n
 * const int constantInteger; \n
 *
 * class ClassName \n
 * { \n
 * private: \n
 *     int memberVariable; \n
 * \n
 * public: \n
 *     ClassName(); \n
 *     inline void MethodName() { int localVariable = 1; } \n
 * }; \n
 * \n
 * void SomeFunction() \n
 * { \n
 *     // four spaces instead of tab \n
 * } \n
 * </code>
 *
 * Expand tabs to four spaces
 *
 * Don't use stdio or iostream unless it has been sanitized by #ifndef NDEBUG\\ printf \\#endif
 *
 * Never use third party libraries which break the previous rule
 *
 * Inside the application, always write full paths to includes, do not use .. etc. For example DO: "core/x-platform/locator.h" DON'T: "../renderer.h"
 *
 * You may #include "core/application.h" and use the Log(""), LogError("") and LogWarning("") macros
 *
 * Don't use any standard library stuff, write your own containers and use those in utils, Ex: core/containers/linkedlist.h
 *
 * Reason for two previous statements: They are implemented differently from compiler to compiler
 *
 * Member variables will show up as red in QtCreator. Data types will be purple and local variables will be dark blue, and methods will be light blue.
 *
 * \section third_party Third Party Licenses
 *
 * This software is based in part on the work of the Independent JPEG Group.
 *
 * -----------------------------------------------------------------------------
 *
 * A C-program for MT19937, with initialization improved 2002/1/26.
 * Coded by Takuji Nishimura and Makoto Matsumoto.
 *
 * Before using, initialize the state by using init_genrand(seed)
 * or init_by_array(init_key, key_length).
 *
 * Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  3. The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * Any feedback is very welcome.
 * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
 * email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
 *
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "core/x-platform/memory.h"
#include "core/x-platform/splash.h"
#include "core/x-platform/arguments.h"
#include "core/x-platform/computedinclude.h"
#include COMPUTED_INCLUDE(PLATFORM_HEADER)

/*! \brief Base class for the application
 *
 * \details Create Application(argc, argv) and call the method Exec()
 * See examples/templates/application/source/main.cpp
 */

class Application : public PLATFORM
{
public:
    Application(int argumentQuantity, char *arguments[]);
    ~Application();

protected:
    Arguments<String> argument;

private:
    void LoadUserData();
    void ParseSettings(String &name, int &windowLength, int &windowHeight,  bool &fullscreen, float &timeScale);
    void ParseScenes();
};

#endif // APPLICATION_H
