Project {
    property stringList defines: [
        "AUDIO_HEADER=modules/audio/null/nullaudio.h",
        "RENDERER_HEADER=modules/renderer/vulkan/vulkanrenderer.h",
  //      "PHYSICS_HEADER=modules/physics/bullet/bulletphysics.h",
        "PHYSICS_HEADER=modules/physics/bounce/bouncephysics.h",
        "SCRIPT_HEADER=modules/script/null/nullscript.h",
        "FILESYSTEM_HEADER=modules/filesystem/std/stdfilesystem.h",
        "AUDIO=Audio::NullAudio",
        "RENDERER=VulkanRenderer",
  //      "PHYSICS=BulletPhysics",
        "PHYSICS=Physics::BouncePhysics",
        "SCRIPT=Script::NullScript",
        "FILESYSTEM=StdFileSystem",
        "VIDEO=THEORA",
        "USE_VULKAN=1"
    ]

    property stringList sdlDefines: [
        "PLATFORM_HEADER=core/platforms/sdl/sdlapplication.h",
        "PLATFORM=SDLApplication"
    ]

    property stringList glfwDefines: [
        "PLATFORM_HEADER=core/platforms/glfw/glfwapplication.h",
        "PLATFORM=GLFWApplication",
        "ES_PROFILE"
    ]

    property stringList windowsDefines: [
        "PLATFORM_HEADER=core/platforms/win/winapplication.h",
        "PLATFORM=WinApplication",
        "WIN32"
    ]

    property stringList staticLibs: [
        "core",
        "vulkanrenderer",
        "stdfilesystem",
        "nullscript",
        "nullaudio",
        "nullrenderer",
        "nullfilesystem",
        "bouncephysics",
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
        "Cocoa",
    ]

    property stringList macosSharedLibs: [
        "m",
        "iconv"
    ]

    property stringList linuxSharedLibs: [
        "dl",
        "pthread"
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
        cpp.cxxLanguageVersion: "c++23"

        files: [
            "source/3rdparty/jsmn.cpp",
            "source/3rdparty/RtMidi.cpp",
            "source/core/containers/array.h",
            "source/core/containers/bitarray.h",
            "source/core/containers/hash.h",
            "source/core/containers/linkedlist.h",
            "source/core/containers/queue.h",
            "source/core/containers/stack.h",
            "source/core/components/actor.cpp",
            "source/core/components/actor.h",
            "source/core/components/background.cpp",
            "source/core/components/background.h",
            "source/core/components/cell.cpp",
            "source/core/components/cell.h",
            "source/core/components/circle.cpp",
            "source/core/components/circle.h",
            "source/core/components/command.cpp",
            "source/core/components/command.h",
            "source/core/components/component.cpp",
            "source/core/components/component.h",
            "source/core/components/cube.cpp",
            "source/core/components/cube.h",
            "source/core/components/grid.cpp",
            "source/core/components/grid.h",
            "source/core/components/line.cpp",
            "source/core/components/line.h",
            "source/core/components/rectangle.cpp",
            "source/core/components/rectangle.h",
            "source/core/components/snowglobecamera.cpp",
            "source/core/components/snowglobecamera.h",
            "source/core/components/fpscamera.cpp",
            "source/core/components/fpscamera.h",
            "source/core/components/fpscounter.cpp",
            "source/core/components/fpscounter.h",
            "source/core/components/arealight.cpp",
            "source/core/components/arealight.h",
            "source/core/components/directionallight.cpp",
            "source/core/components/directionallight.h",
            "source/core/components/pointlight.cpp",
            "source/core/components/pointlight.h",
            "source/core/components/spotlight.cpp",
            "source/core/components/spotlight.h",
            "source/core/components/camera.cpp",
            "source/core/components/camera.h",
            "source/core/components/text.cpp",
            "source/core/components/text.h",
            "source/core/components/mesh.cpp",
            "source/core/components/mesh.h",
            "source/core/components/sprite.cpp",
            "source/core/components/sprite.h",
            "source/core/components/sphere.cpp",
            "source/core/components/sphere.h",
            "source/core/components/triangle.cpp",
            "source/core/components/triangle.h",
            "source/core/components/sound.cpp",
            "source/core/components/sound.h",
            "source/core/components/videocapture.cpp",
            "source/core/components/videocapture.h",
            "source/core/components/tankcontrols.cpp",
            "source/core/components/tankcontrols.h",
            "source/core/components/laddercontrols.cpp",
            "source/core/components/laddercontrols.h",
            "source/core/components/skipcutscene.cpp",
            "source/core/components/skipcutscene.h",
            "source/core/x-platform/curves.h",
            "source/core/x-platform/mat.cpp",
            "source/core/x-platform/mat.h",
            "source/core/x-platform/generatedscene.cpp",
            "source/core/x-platform/generatedscene.h",
            "source/core/x-platform/parserjsmn.cpp",
            "source/core/x-platform/parserjsmn.h",
            "source/core/x-platform/parserjson.cpp",
            "source/core/x-platform/parserjson.h",
            "source/core/x-platform/input.cpp",
            "source/core/x-platform/input.h",
            "source/core/x-platform/locator.cpp",
            "source/core/x-platform/locator.h",
            "source/core/x-platform/ptr.h",
            "source/core/x-platform/services.cpp",
            "source/core/x-platform/services.h",
            "source/core/x-platform/splash.cpp",
            "source/core/x-platform/splash.h",
            "source/core/x-platform/memory.cpp",
            "source/core/x-platform/memory.h",
            "source/core/x-platform/random.cpp",
            "source/core/x-platform/random.h",
            "source/core/x-platform/string.cpp",
            "source/core/x-platform/string.h",
            "source/core/x-platform/url.cpp",
            "source/core/x-platform/url.h",
            "source/core/x-platform/keyframe.cpp",
            "source/core/x-platform/keyframe.h",
            "source/core/x-platform/animation.cpp",
            "source/core/x-platform/animation.h",
            "source/core/x-platform/joint.cpp",
            "source/core/x-platform/joint.h",
            "source/core/application.cpp",
            "source/core/application.h",
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

        Properties {
            condition: qbs.targetOS.contains("linux")

            cpp.defines: project.defines.concat(project.glfwDefines)
            cpp.includePaths: ["source", "include/linux"]
        }

        Group {
            name: "GLFW files"
            condition: qbs.targetOS.contains("linux")

            files: [
                "source/core/platforms/glfw/gl.cpp",
                "source/core/platforms/glfw/glfwapplication.cpp",
                "source/core/platforms/glfw/glfwapplication.h",
                "source/core/platforms/glfw/glfwdebug.cpp",
                "source/core/platforms/glfw/glfwdebug.h",
                "source/core/platforms/glfw/glfwtime.cpp",
                "source/core/platforms/glfw/glfwtime.h"
            ]
        }
        Group {
            name: "Windows files"

            condition: qbs.targetOS.contains("windows")

            files: [
                "source/core/platforms/win/winapplication.cpp",
                "source/core/platforms/win/winapplication.h",
                "source/core/platforms/win/winrenderer.cpp",
                "source/core/platforms/win/winrenderer.h",
                "source/core/platforms/win/windebug.cpp",
                "source/core/platforms/win/windebug.h",
                "source/core/platforms/win/wintime.cpp",
                "source/core/platforms/win/wintime.h",
                "source/core/platforms/win/gl.cpp",
            ]
        }

        Group {
            name: "macOS files"

            condition: qbs.targetOS.contains("macos")

            files: [
                "source/core/platforms/sdl/sdlapplication.cpp",
                "source/core/platforms/sdl/sdlapplication.h",
                "source/core/platforms/sdl/sdlrender.cpp",
                "source/core/platforms/sdl/sdlrender.h",
                "source/core/platforms/sdl/sdldebug.cpp",
                "source/core/platforms/sdl/sdldebug.h",
                "source/core/platforms/sdl/sdltime.cpp",
                "source/core/platforms/sdl/sdltime.h",
                "source/core/platforms/sdl/gl.cpp",
                "source/core/platforms/sdl/gl.h",
            ]
        }
    }

    Product {
        name: "portaudioaudio"
        type: "staticlibrary"

        files: [
            "source/modules/audio/portaudioaudio/portaudioaudio.cpp",
            "source/modules/audio/portaudioaudio/portaudioaudio.h",
            "source/modules/audio/portaudioaudio/portaudiodata.cpp",
            "source/modules/audio/portaudioaudio/portaudiodata.h",
        ]
    }

    Product {
        name: "stdfilesystem"
        type: "staticlibrary"

        files: [
            "source/modules/filesystem/std/stdfile.cpp",
            "source/modules/filesystem/std/stdfile.h",
            "source/modules/filesystem/std/stdfilesystem.cpp",
            "source/modules/filesystem/std/stdfilesystem.h",
        ]
    }

    Product {
        name: "dx11renderer"
        type: "staticlibrary"

        files: [
            "source/modules/renderer/dx11/dx11renderer.cpp",
            "source/modules/renderer/dx11/dx11renderer.h",
            "source/modules/renderer/dx11/dx11drawable.cpp",
            "source/modules/renderer/dx11/dx11drawable.h",
        ]
    }

    Product {
        name: "gles3renderer"
        type: "staticlibrary"

        files: [
            "source/modules/renderer/gles3/gles3renderer.cpp",
            "source/modules/renderer/gles3/gles3renderer.h",
            "source/modules/renderer/gles3/gles3drawable.cpp",
            "source/modules/renderer/gles3/gles3drawable.h",
            "source/modules/renderer/gles3/gles3shader.cpp",
            "source/modules/renderer/gles3/gles3shader.h",
            "source/modules/renderer/gles3/gles3texture.cpp",
            "source/modules/renderer/gles3/gles3texture.h",
        ]
    }

    Product {
        name: "nullfilesystem"
        type: "staticlibrary"

        files: [
            "source/modules/filesystem/null/nullfile.cpp",
            "source/modules/filesystem/null/nullfile.h",
            "source/modules/filesystem/null/nullfilesystem.cpp",
            "source/modules/filesystem/null/nullfilesystem.h",
        ]
    }

    Product {
        name: "nullrenderer"
        type: "staticlibrary"

        files: [
            "source/modules/renderer/null/nullrenderer.cpp",
            "source/modules/renderer/null/nullrenderer.h",
            "source/modules/renderer/null/nulldrawable.cpp",
            "source/modules/renderer/null/nulldrawable.h",
        ]
    }

    Product {
        name: "nullaudio"
        type: "staticlibrary"

        files: [
            "source/modules/audio/null/nullaudio.cpp",
            "source/modules/audio/null/nullaudio.h",
        ]
    }

    Product {
        name: "nullphysics"
        type: "staticlibrary"

        files: [
            "source/modules/physics/null/nullphysics.cpp",
            "source/modules/physics/null/nullphysics.h",
        ]
    }

    Product {
        name: "bouncephysics"
        type: "staticlibrary"

        files: [
            "source/modules/physics/bounce/bouncephysics.cpp",
            "source/modules/physics/bounce/bouncephysics.h",
        ]
    }

    Product {
        name: "bulletphysics"
        type: "staticlibrary"

        files: [
            "source/modules/physics/bullet/bulletphysics.cpp",
            "source/modules/physics/bullet/bulletphysics.h",
        ]
    }

    Product {
        name: "sdlaudio"
        type: "staticlibrary"

        files: [
            "source/modules/audio/sdl/sdlaudio.cpp",
            "source/modules/audio/sdl/sdlaudio.h",
        ]
    }

    Product {
        name: "vulkanrenderer"
        type: "staticlibrary"

        cpp.cxxLanguageVersion: "c++23"

        files: [
            "source/modules/renderer/vulkan/vk_mem_alloc.h",
            "source/modules/renderer/vulkan/vulkanrenderer.cpp",
            "source/modules/renderer/vulkan/vulkanrenderer.h",
            "source/modules/renderer/vulkan/vulkandrawable.cpp",
            "source/modules/renderer/vulkan/vulkandrawable.h",
            "source/modules/renderer/vulkan/vulkanshader.cpp",
            "source/modules/renderer/vulkan/vulkanshader.h",
            "source/modules/renderer/vulkan/vulkantexture.cpp",
            "source/modules/renderer/vulkan/vulkantexture.h",
        ]
    }
}
