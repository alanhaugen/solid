Project {
    property stringList defines: [
        "AUDIO_HEADER=modules/audio/portaudioaudio/portaudioaudio.h",
        "RENDERER_HEADER=modules/renderer/gles2/gles2renderer.h",
        "PHYSICS_HEADER=modules/physics/null/nullphysics.h",
        "SCRIPT_HEADER=modules/script/null/nullscript.h",
        "FILESYSTEM_HEADER=modules/filesystem/std/stdfilesystem.h",
        "AUDIO=PortaudioAudio",
        "RENDERER=GLES2Renderer",
        "PHYSICS=NullPhysics",
        "SCRIPT=NullScript",
        "FILESYSTEM=StdFileSystem",
        "THEORA"
    ]

    property stringList sdlDefines: [
        "KEYMAP_FIRSTKEY=97",
        "PLATFORM_HEADER=core/platforms/sdl/sdlapplication.h",
        "PLATFORM=SDLApplication"
    ]

    property stringList windowsDefines: [
        "KEYMAP_FIRSTKEY=65",
        "PLATFORM_HEADER=core/platforms/win/winapplication.h",
        "PLATFORM=WinApplication"
    ]

    property stringList staticLibs: [
        "core",
        "vorbisenc",
        "vorbisfile",
        "vorbis",
        "ogg",
        "portaudioaudio",
        "portaudio_static",
        "sndfile",
        "gles2renderer",
        "stdfilesystem",
        "theora",
        "theoradec",
        "theoraenc",
        "nullscript",
        "nullaudio",
        "nullrenderer",
        "nullfilesystem",
        "nullphysics"
    ]

    property stringList macosFrameworks: [
        "CoreMIDI",
        "CoreFoundation",
        "AudioToolbox",
        "CoreAudio",
        "Carbon",
        "ForceFeedback",
        "CoreVideo",
        "AppKit",
        "OpenGL",
        "Metal",
        "IOKit",
        "Cocoa"
    ]

    property stringList macosSharedLibs: [
        "m",
        "iconv"
    ]

    property stringList windowsSharedLibs: [
        "opengl32",
        "winmm",
        "hid",
        "setupapi",
        "gdi32"
    ]

    Product {
        name: "core"
        type: "staticlibrary"

        files: [
            "source/3rdparty/jsmn.cpp",
            "source/3rdparty/RtMidi.cpp",
            "source/core/components/actor.cpp",
            "source/core/components/background.cpp",
            "source/core/components/cell.cpp",
            "source/core/components/circle.cpp",
            "source/core/components/command.cpp",
            "source/core/components/component.cpp",
            "source/core/components/cube.cpp",
            "source/core/components/grid.cpp",
            "source/core/components/line.cpp",
            "source/core/components/rectangle.cpp",
            "source/core/components/snowglobecamera.cpp",
            "source/core/components/fpscamera.cpp",
            "source/core/components/fpscounter.cpp",
            "source/core/components/arealight.cpp",
            "source/core/components/directionallight.cpp",
            "source/core/components/pointlight.cpp",
            "source/core/components/spotlight.cpp",
            "source/core/components/camera.cpp",
            "source/core/components/text.cpp",
            "source/core/components/mesh.cpp",
            "source/core/components/sprite.cpp",
            "source/core/components/sphere.cpp",
            "source/core/components/triangle.cpp",
            "source/core/components/collisionbox.cpp",
            "source/core/components/sound.cpp",
            "source/core/components/videocapture.cpp",
            "source/core/components/tankcontrols.cpp",
            "source/core/components/laddercontrols.cpp",
            "source/core/components/skipcutscene.cpp",
            "source/core/x-platform/mat.cpp",
            "source/core/x-platform/generatedscene.cpp",
            "source/core/x-platform/parserjsmn.cpp",
            "source/core/x-platform/parserjson.cpp",
            "source/core/x-platform/input.cpp",
            "source/core/x-platform/locator.cpp",
            "source/core/x-platform/services.cpp",
            "source/core/x-platform/splash.cpp",
            "source/core/x-platform/memory.cpp",
            "source/core/x-platform/pixmap.cpp",
            "source/core/x-platform/random.cpp",
            "source/core/x-platform/string.cpp",
            "source/core/x-platform/url.cpp",
            "source/core/x-platform/keyframe.cpp",
            "source/core/x-platform/animation.cpp",
            "source/core/x-platform/joint.cpp",
            "source/core/application.cpp",
            "source/tools/3dformat.py",
        ]

        Depends { name: "cpp" }

        Properties {
            condition: qbs.targetOS.contains("windows")

            cpp.defines: project.defines.concat(project.windowsDefines)
            cpp.includePaths: ["source", "include/mingw32"]
        }

        Properties {
            condition: qbs.targetOS.contains("macos")

            cpp.defines: project.defines.concat(project.sdlDefines)
            cpp.includePaths: ["source", "include/darwin"]
        }

        Group {
            name: "Windows files"

            condition: qbs.targetOS.contains("windows")

            files: [
                "source/core/platforms/win/winapplication.cpp",
                "source/core/platforms/win/winrenderer.cpp",
                "source/core/platforms/win/windebug.cpp",
                "source/core/platforms/win/wintime.cpp",
                "source/core/platforms/win/gl.cpp",
                "source/core/platforms/win/wgl.cpp"
            ]
        }

        Group {
            name: "macOS files"

            condition: qbs.targetOS.contains("macos")

            files: [
                "source/core/platforms/sdl/sdlapplication.cpp",
                "source/core/platforms/sdl/sdlrender.cpp",
                "source/core/platforms/sdl/sdldebug.cpp",
                "source/core/platforms/sdl/sdltime.cpp",
                "source/core/platforms/sdl/gl.cpp"
            ]
        }

        Group {
            name: "Cafe files"

            condition: qbs.targetOS.contains("cygwin")

            files: [
                "source/core/platforms/cafe/cafeapplication.cpp",
                "source/core/platforms/cafe/cafedebug.cpp",
                "source/core/platforms/cafe/cafetime.cpp",
                "source/core/platforms/cafe/parsernlib.cpp"
            ]
        }
    }

    Product {
        name: "cafefilesystem"
        type: "staticlibrary"

        condition: qbs.targetOS.contains("cygwin")

        files: [
            "source/modules/filesystem/cafefs/cafefile.cpp",
            "source/modules/filesystem/cafefs/cafefilesystem.cpp"
        ]
    }

    Product {
        name: "gx2renderer"
        type: "staticlibrary"

        condition: qbs.targetOS.contains("cygwin")

        files: [
            "source/modules/renderer/gx2/gx2renderer.cpp",
            "source/modules/renderer/gx2/gx2drawable.cpp",
            "source/modules/renderer/gx2/gx2shader.cpp"
        ]
    }

    Product {
        name: "portaudioaudio"
        type: "staticlibrary"

        files: [
            "source/modules/audio/portaudioaudio/portaudioaudio.cpp",
            "source/modules/audio/portaudioaudio/portaudiodata.cpp"
        ]
    }

    Product {
        name: "stdfilesystem"
        type: "staticlibrary"

        files: [
            "source/modules/filesystem/std/stdfile.cpp",
            "source/modules/filesystem/std/stdfilesystem.cpp"
        ]
    }

    Product {
        name: "gles2renderer"
        type: "staticlibrary"

        files: [
            "source/modules/renderer/gles2/gles2renderer.cpp",
            "source/modules/renderer/gles2/gles2drawable.cpp",
            "source/modules/renderer/gles2/gles2shader.cpp",
            "source/modules/renderer/gles2/gles2texture.cpp"
        ]
    }

    Product {
        name: "nullfilesystem"
        type: "staticlibrary"

        files: [
            "source/modules/filesystem/null/nullfile.cpp",
            "source/modules/filesystem/null/nullfilesystem.cpp"
        ]
    }

    Product {
        name: "nullrenderer"
        type: "staticlibrary"

        files: [
            "source/modules/renderer/null/nullrenderer.cpp",
            "source/modules/renderer/null/nulldrawable.cpp"
        ]
    }

    Product {
        name: "nullaudio"
        type: "staticlibrary"

        files: [
            "source/modules/audio/null/nullaudio.cpp"
        ]
    }

    Product {
        name: "nullphysics"
        type: "staticlibrary"

        files: [
            "source/modules/physics/null/nullphysics.cpp"
        ]
    }
}
