#include "locator.h"

ITime *Locator::time = NULL;
IDebug *Locator::debug = NULL;
Input Locator::input = Input();
Random Locator::random = Random(); // default seed

NullAudio Locator::nullAudio;
NullFileSystem Locator::nullFileSystem;
Physics::NullPhysics Locator::nullPhysics;
Renderer::NullRenderer Locator::nullRenderer;
NullScript Locator::nullScript;

IAudio *Locator::audio = &Locator::nullAudio;
IFileSystem *Locator::filesystem = &Locator::nullFileSystem;
Physics::IPhysics *Locator::physics = &Locator::nullPhysics;
Renderer::IRenderer *Locator::renderer = &Locator::nullRenderer;
IScript *Locator::script = &Locator::nullScript;

Array<glm::mat4> Locator::viewProjections = Array<glm::mat4>();
Array<glm::vec4> Locator::viewports = Array<glm::vec4>();
float Locator::deltaTime;

void Locator::SetTime(ITime *_time)
{
    if (_time != NULL)
    {
        time = _time;
    }
}

void Locator::SetDebug(IDebug *_debug)
{
    if (_debug != NULL)
    {
        debug = _debug;
    }
}

void Locator::SetAudio(IAudio *_audio)
{
    if (_audio == NULL)
    {
        // Revert to null system
        audio = &nullAudio;
    }
    else
    {
        audio = _audio;
    }
}

void Locator::SetFileSystem(IFileSystem *_fileSystem)
{
    if (_fileSystem == NULL)
    {
        // Revert to null system
        filesystem = &nullFileSystem;
    }
    else
    {
        filesystem = _fileSystem;
    }
}

void Locator::SetPhysics(Physics::IPhysics *_physics)
{
    if (_physics == NULL)
    {
        // Revert to null system
        physics = &nullPhysics;
    }
    else
    {
        physics = _physics;
    }
}

void Locator::SetRenderer(Renderer::IRenderer *_renderer)
{
    if (_renderer == NULL)
    {
        // Revert to null system
        renderer = &nullRenderer;
    }
    else
    {
        renderer = _renderer;
    }
}

void Locator::SetScript(IScript *_script)
{
    if (_script == NULL)
    {
        // Revert to null system
        script = &nullScript;
    }
    else
    {
        script = _script;
    }
}
