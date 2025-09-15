import "../../../solid/solid.qbs" as solid

solid {
    Application {
        name: "App"
        cpp.cxxLanguageVersion: "c++23"

        files: [
            "source/main.cpp",
            "source/main.h",
        ]

        property stringList includePaths: "../../../solid/source"

        Depends { name: "cpp" }
        Depends { name: "core" }
        Depends { name: "nullrenderer"  }
        Depends { name: "nullphysics"  }
        Depends { name: "nullaudio"  }
        Depends { name: "nullphysics"  }
        Depends { name: "nullfilesystem"  }
        Depends { name: "nullscript" }
        Depends { name: "vulkanrenderer"  }
        Depends { name: "stdfilesystem"  }
        Depends { name: "portaudioaudio"  }
        Depends { name: "nullphysics" }
        Depends { name: "bouncephysics" }

        Properties {
            condition: qbs.targetOS.contains("macos")

            cpp.frameworks: {
                if (qbs.architecture.includes("arm64"))
                    return macosFrameworks.concat(
                           "CoreHaptics",
                           "MediaPlayer",
                           "GameController",
                           "QuartzCore",
                           "IOSurface")
                return macosFrameworks
            }

            cpp.dynamicLibraries: macosSharedLibs
            cpp.staticLibraries: staticLibs.concat("SDL2", "MoltenVK")

            cpp.libraryPaths: [project.buildDirectory, "../solid/lib/debug/darwin/" + qbs.architecture]
            cpp.includePaths: includePaths.concat("../solid/include/darwin")
            cpp.defines: project.defines.concat(project.sdlDefines)
        }

        Properties {
            condition: qbs.targetOS.contains("linux")

            cpp.dynamicLibraries: linuxSharedLibs
            cpp.staticLibraries: staticLibs.concat("glfw3")

            cpp.libraryPaths: [project.buildDirectory, "../solid/lib/debug/linux" + qbs.architecture]
            cpp.includePaths: includePaths.concat("../solid/include/linux")
            cpp.defines: project.defines.concat(project.glfwDefines)
        }

        Properties {
            condition: qbs.targetOS.contains("windows")

            cpp.dynamicLibraries: windowsSharedLibs
            cpp.staticLibraries: staticLibs

            cpp.libraryPaths: [project.buildDirectory, "../../../solid/lib/debug/mingw32/x86_64"]
            cpp.includePaths: includePaths.concat("../../../solid/include/mingw32")
            cpp.defines: project.defines.concat(project.windowsDefines)
        }
    }
}
