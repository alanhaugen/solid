#include "sdlapplication.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SDLApplication::SDLApplication()
{
    SetRenderer(new SDLRender());
    SetDebug(new SDLDebug());
    SetAudio(new AUDIO());
    SetFileSystem(new FILESYSTEM());
    SetScript(new SCRIPT());
    SetPhysics(new PHYSICS());

    // SDL initialization
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        LogError(SDL_GetError());
    }
}

SDLApplication::~SDLApplication()
{
    SDL_Quit();
}

ITime *SDLApplication::GetTime(const char *timeName, float timeScale)
{
    return new SDLTime(timeName, timeScale);
}

void SDLApplication::MessageLoop(SDL_Event event)
{
    switch(event.type)
    {
    case SDL_QUIT:
        isAlive = false;
        break;

    case SDL_KEYDOWN:
        input.OnButton(event.key.keysym.sym, true);
        break;

    case SDL_KEYUP:
        input.OnButton(event.key.keysym.sym, false);
        break;

    case SDL_MOUSEMOTION:
        input.Mouse.dx = event.motion.x - input.Mouse.x;
        input.Mouse.dy = event.motion.y - input.Mouse.y;

        input.Mouse.x = event.motion.x;
        input.Mouse.y = event.motion.y;
        break;

    case SDL_MOUSEBUTTONDOWN:
        input.Mouse.Up = false;
        input.Mouse.Down = true;
        input.Mouse.Pressed = true;
        break;

    case SDL_MOUSEBUTTONUP:
        if (input.Mouse.Down)
        {
            input.Mouse.Released = true;
        }

        input.Mouse.Up = true;
        input.Mouse.Down = false;
        break;

    case SDL_WINDOWEVENT:
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
            renderer->Resize(event.window.data1, event.window.data2);
        }
        break;
    }
}

bool SDLApplication::Init()
{
    // Log what runtime this is and the date and time
    Log("SDL Runtime");
    Log(String(__DATE__) + " " + String(__TIME__));

    return true;
}

float SDLApplication::MainLoop()
{
    float sleepMilliseconds = 0.0f;

    if (SDL_PollEvent(&event))
    {
        MessageLoop(event);
    }
    else
    {
        deltaTime = time->TimeSinceStarted();
        sleepMilliseconds = 16.6f - deltaTime;

        UpdateServices();

        // Update this to make sure Released and pressed aren't always true
        input.Mouse.Released = false;
        input.Mouse.Pressed  = false;

        // Reset mouse movement delta
        input.Mouse.dx = 0;
        input.Mouse.dy = 0;

        // Put mouse in center if dragging
        if (input.Mouse.Dragging)
        {
            SDL_WarpMouseInWindow(SDLrender->GetDisplay(),
                                  renderer->windowWidth / 2,
                                  renderer->windowHeight / 2);

            input.Mouse.x = (renderer->windowWidth / 2);
            input.Mouse.y = (renderer->windowHeight / 2);
        }

        // Show or hide mouse cursor
        if (input.Mouse.Hidden)
        {
            SDL_ShowCursor(SDL_DISABLE);
        }
        else
        {
            SDL_ShowCursor(SDL_ENABLE);
        }
    }

    return sleepMilliseconds;
}

bool SDLApplication::Exec()
{
    // Time since application started
    ITime *runtime = GetTime("Runtime");

    // Set display for debug error messages
    SDLdebug = dynamic_cast<SDLDebug*>(debug);
    SDLrender = dynamic_cast<SDLRender*>(renderer);

    SDLdebug->SetDisplay(SDLrender->GetDisplay());

    while (isAlive)
    {
#ifdef __EMSCRIPTEN__OFF
        emscripten_set_main_loop(MainLoop, 0, 1);
#else
        float sleepMilliseconds = MainLoop();

        // Release time back to other apps
        if (sleepMilliseconds > 0.0f)
        {
            SDL_Delay(sleepMilliseconds);

            if (fullscreen)
            {
                // SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0
                SDL_SetWindowFullscreen(SDLrender->GetDisplay(), SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
        }
#endif
    }

    delete runtime;

    return false;
}
