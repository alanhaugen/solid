#include "application.h"
#include "core/x-platform/parserjson.h"
#include <time.h> // TODO: Remove?

Application::Application(int argumentQuantity, char *arguments[], String appName, int resX, int resY)
    : PLATFORM(), argument(argumentQuantity, arguments)
{
    isAlive = Init();
    LoadUserData(appName, resX, resY);
}

Application::~Application()
{
    delete renderer;
    delete debug;
    delete audio;
    delete filesystem;
    delete script;
    delete physics;

    filesystem = NULL;
    renderer   = NULL;
    debug      = NULL;
    audio      = NULL;
    script     = NULL;
    physics    = NULL;
}

void Application::LoadUserData(String appName, int resX, int resY)
{
    String name = appName;
    int windowLength = resX;
    int windowHeight = resY;

    bool fullscreen  = false;
    float timeScale  = 1.0;

    // TODO: Remove?
    random.SetRandomSeed(::time(NULL));

    // Process command line parameters
    /*argument.SetArgument<int>("--window-length", windowLength);
    argument.SetArgument<int>("--window-height", windowHeight);
    argument.GetArgument<bool>("--paused");
    argument.SetArgument<int>("--scene", scene_);
    argument.SetArgument<bool>("--fullscreen", fullscreen);
    //int *monitor      = argument.GetArgument<int>("--monitor");
    //argument.GetArgument<float>("--time-scale");
    //bool *testRun     = argument.GetArgument<bool>("--test-run");*/

    // Initialize filesystem
    filesystem->Init();

    // Parse settings json
    //ParseSettings(name, windowLength, windowHeight, fullscreen, timeScale);

    // Initialize renderer
    if (renderer->Init(fullscreen, name.ToChar(), windowLength, windowHeight) == false)
    {
        LogError( String("Failed to initialize renderer") );
        Quit();
    }

    // Initialize audio
    audio->Init();

    // Universal time since start of frame
    SetTime(GetTime("Timestep", timeScale));

    // Add scenes
    //AddScene(new Splash());
    ParseScenes();

    // Load languages

    // Setup keys
    input.Define("START",  input.Key.ESCAPE);
    input.Define("SELECT", input.Key.ENTER);
    input.Define("UP",     input.Key.UP);
    input.Define("DOWN",   input.Key.DOWN);
    input.Define("LEFT",   input.Key.LEFT);
    input.Define("RIGHT",  input.Key.RIGHT);
    input.Define("FIRE",   input.Key.SPACE);
    input.Define("JUMP",   input.Key.CTRL);
    input.Define("ALT1",   input.Key.SHIFT);
    input.Define("ALT2",   input.Key.ALT);
}

void Application::ParseSettings(String &name, int &windowLength, int &windowHeight, bool &fullscreen, float &timeScale)
{
    ParserJSON jsonSettings("data/project.json");

    jsonSettings.AddObject("settings");

    if (jsonSettings.GetNextObject() == "settings")
    {
        name = jsonSettings.GetString("name");
        windowLength = jsonSettings.GetNumber("windowLength");
        windowHeight = jsonSettings.GetNumber("windowHeight");
        fullscreen = jsonSettings.GetNumber("fullscreen");
        //timeScale = jsonSettings.GetNumber("timeScale");
    }
}

void Application::ParseScenes()
{
    // Read json project file
    ParserJSON jsonSettings("data/project.json");

    Array<String> scenePaths;
    Array<int> sceneIds;

    jsonSettings.AddObject("scenes");

    if (jsonSettings.GetNextObject() == "scenes")
    {
        while (jsonSettings.isInArray())
        {
            sceneIds.Add(jsonSettings.GetNumber("id"));
            scenePaths.Add("data/" + jsonSettings.GetString("filename"));
        }
    }

    for (unsigned int i = 0; i < sceneIds.Size(); i++)
    {
        AddScene(scenePaths[sceneIds[i]-1]);
    }
}
