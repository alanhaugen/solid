#include "glfwapplication.h"

GLFWApplication::GLFWApplication()
{
    SetDebug(new GLFWDebug());
    SetRenderer(new RENDERER());
    SetAudio(new AUDIO());
    SetFileSystem(new FILESYSTEM());
    SetScript(new SCRIPT());
    SetPhysics(new PHYSICS());
}

GLFWApplication::~GLFWApplication()
{
}

ITime *GLFWApplication::GetTime(const char *timeName, float timeScale)
{
    return new GLFWTime(timeName, timeScale);
}

/*void GLFWApplication::MessageLoop(SDL_Event event)
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
            //SDL_DisplayMode mode;
            //SDL_GetDisplayMode(event.display.display, 0, &mode);
            //SDL_SetWindowSize(SDLrender->GetDisplay(), mode.w, mode.h);
            //renderer->Resize(mode.w, mode.h);
            renderer->Resize(event.window.data1, event.window.data2);
            renderer->actualWindowWidth  = event.window.data1;
            renderer->actualWindowHeight = event.window.data2;
            //SDL_RaiseWindow(SDL_GetWindowFromID(event.window.windowID));
            //SDL_SetWindowInputFocus(SDL_GetWindowFromID(event.window.windowID));
            //SDL_SetWindowMouseGrab(SDLrender->GetDisplay(), SDL_bool(true));
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
}*/

bool GLFWApplication::Init()
{
    // Log what runtime this is and the date and time
    Log("GLFW Runtime");
    Log(String(__DATE__) + " " + String(__TIME__));

    if (!glfwInit())
    {
        LogError("Failed to initialise glfw");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(640, 480, "Solid", NULL, NULL);

    if (!window)
    {
        LogError("Failed to create window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);

    return true;
}

bool GLFWApplication::Exec()
{
    // Time since application started
    ITime *runtime = GetTime("Runtime");

    while (!glfwWindowShouldClose(window))//isAlive)
    {
        float sleepMilliseconds;// = MainLoop();
        UpdateServices();

        // Release time back to other apps
        if (sleepMilliseconds > 0.0f)
        {
            //SDL_Delay(sleepMilliseconds);
	    
            time->Reset();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete runtime;

    glfwTerminate();

    return false;
}

