#ifndef GLFWAPPLICATION_H
#define GLFWAPPLICATION_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "core/x-platform/services.h"
#include "glfwtime.h"
#include "glfwdebug.h"
#include "core/x-platform/computedinclude.h"
#include COMPUTED_INCLUDE(AUDIO_HEADER)
#include COMPUTED_INCLUDE(SCRIPT_HEADER)
#include COMPUTED_INCLUDE(PHYSICS_HEADER)
#include COMPUTED_INCLUDE(FILESYSTEM_HEADER)
#include COMPUTED_INCLUDE(RENDERER_HEADER)

class GLFWApplication : public Services
{
public:
    GLFWApplication();
    ~GLFWApplication();

    bool Exec();

    static ITime *GetTime(const char *timeName = "Time", float timeScale = 1.0);

protected:
    bool Init();

    GLFWwindow* window;

    //GLFWDebug *SDLdebug;
    //GLFWRender *SDLrender;
};

#endif // GLFWAPPLICATION_H
