#include "sdlapplication.h"
#include "core/x-platform/splash.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __MACOSX__
#include <mach/mach_time.h>

void precise_delay(double seconds) {
    uint64_t now = mach_absolute_time();
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);

    // Convert seconds to nanoseconds
    uint64_t elapsed_ns = (uint64_t)(seconds * 1e9);
    // Convert to absolute time units
    elapsed_ns = elapsed_ns * info.denom / info.numer;

    mach_wait_until(now + elapsed_ns);
}
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

    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
            Log("Gamepad detected");
            break;
        }
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

    case SDL_CONTROLLERDEVICEADDED:
        if (!controller)
        {
            controller = SDL_GameControllerOpen(event.cdevice.which);
        }
        break;
    case SDL_CONTROLLERDEVICEREMOVED:
        if (controller && event.cdevice.which == SDL_JoystickInstanceID(
                    SDL_GameControllerGetJoystick(controller)))
        {
            SDL_GameControllerClose(controller);
            controller = SDL_GameControllerOpen(0);
        }
        break;
    case SDL_CONTROLLERBUTTONDOWN:
        switch (event.cbutton.button)
        {
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X:
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y:
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A:
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B:
            input.OnButton(input.Key.ENTER, true);
            input.OnButton(input.Key.SPACE, true);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP:
            input.OnButton(input.Key.UP, true);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            input.OnButton(input.Key.DOWN, true);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            input.OnButton(input.Key.LEFT, true);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            input.OnButton(input.Key.RIGHT, true);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START:
            input.OnButton(input.Key.P, true);
            break;
        }
        break;
    case SDL_CONTROLLERBUTTONUP:
        switch (event.cbutton.button)
        {
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X:
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y:
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A:
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B:
            input.OnButton(input.Key.ENTER, false);
            input.OnButton(input.Key.SPACE, false);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP:
            input.OnButton(input.Key.UP, false);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            input.OnButton(input.Key.DOWN, false);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            input.OnButton(input.Key.LEFT, false);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            input.OnButton(input.Key.RIGHT, false);
            break;
        case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START:
            input.OnButton(input.Key.P, false);
            break;
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
        sleepMilliseconds = FRAME_TIME_MS - deltaTime;

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

        // Fullscreen or not fullscreen
        if (input.Pressed(input.Key.F) && input.Held(input.Key.LWIN))
        {
            fullscreen = !fullscreen;
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
    // Add a splash screen if there are no scenes
    if (scenes.Empty())
    {
        AddScene(new Splash());
    }

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
#ifdef __MACOSX__
            precise_delay(sleepMilliseconds / 1000.0f);
#else
            SDL_Delay(sleepMilliseconds);
#endif
            time->Reset();

            if (fullscreen)
            {
                // SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0
                SDL_SetWindowFullscreen(SDLrender->GetDisplay(), SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            else
            {
                SDL_SetWindowFullscreen(SDLrender->GetDisplay(), 0);
            }
        }
#endif
    }

    delete runtime;

    return false;
}
