#ifndef LOCATOR_H
#define LOCATOR_H

#include <cstddef>
#include "time.h"
#include "debug.h"
#include "core/x-platform/ptr.h"
#include "core/x-platform/string.h"
#include "core/x-platform/input.h"
#include "core/x-platform/random.h"
#include "modules/audio/null/nullaudio.h"
#include "modules/filesystem/null/nullfilesystem.h"
#include "modules/physics/null/nullphysics.h"
#include "modules/renderer/null/nullrenderer.h"
#include "modules/script/null/nullscript.h"

class Locator
{
public:
    static void SetTime(ITime *_time);
    static void SetDebug(IDebug *_debug);

    static void SetAudio(IAudio *_audio);
    static void SetFileSystem(IFileSystem *_fileSystem);
    static void SetPhysics(IPhysics *_physics);
    static void SetRenderer(IRenderer *_renderer);
    static void SetScript(IScript *_script);

    static ITime *time;
    static IDebug *debug;
    static IAudio *audio;
    static IFileSystem *filesystem;
    static IPhysics *physics;
    static IRenderer *renderer;
    static IScript *script;
    static Random random;
    static Input input;

    static Array<glm::mat4> viewProjections;
    static Array<glm::vec4> viewports;

    static float deltaTime;

private:
    static NullAudio nullAudio;
    static NullFileSystem nullFileSystem;
    static NullPhysics nullPhysics;
    static NullRenderer nullRenderer;
    static NullScript nullScript;
};

//! Macro to easily access the debug singleton and Log a message
#define Log(string) if(Locator::debug) Locator::debug->Write(String(string).ToChar())

//! Macro to easily access the debug singleton and Log a warning
#define LogWarning(string) if(Locator::debug) Locator::debug->Warning( (String("Warning ") + String(__FILE__) + "(" + String(__LINE__) + "): " + String(string) ).ToChar() )

//! Macro to easily access the debug singleton and Log an error
#define LogError(string) if(Locator::debug) Locator::debug->Error( (String("Error ") + String(__FILE__) + "(" + String(__LINE__) + "): " + String(string)).ToChar() )

#endif // LOCATOR_H
