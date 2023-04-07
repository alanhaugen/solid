#ifndef SDLAPPLICATION_H
#define SDLAPPLICATION_H

#include <SDL.h>
#include "core/x-platform/services.h"
#include "core/x-platform/arguments.h"
#include "sdltime.h"
#include "sdldebug.h"
#include "sdlrender.h"
#include COMPUTED_INCLUDE(AUDIO_HEADER)
#include COMPUTED_INCLUDE(SCRIPT_HEADER)
#include COMPUTED_INCLUDE(PHYSICS_HEADER)
#include COMPUTED_INCLUDE(FILESYSTEM_HEADER)

class SDLApplication : public Services
{
public:
    SDLApplication();
    ~SDLApplication();

    bool Exec();

    static ITime *GetTime(const char *timeName = "Time", float timeScale = 1.0);

protected:
    bool Init();

    void MessageLoop(SDL_Event event);
};

#endif // SDLAPPLICATION_H
