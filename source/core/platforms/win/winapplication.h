#ifndef WINAPPLICATION_H
#define WINAPPLICATION_H

#include "wintypes.h"
#include "core/x-platform/services.h"
#include "winrenderer.h"
#include "windebug.h"
#include "wintime.h"
#include "core/x-platform/computedinclude.h"
#include COMPUTED_INCLUDE(AUDIO_HEADER)
#include COMPUTED_INCLUDE(SCRIPT_HEADER)
#include COMPUTED_INCLUDE(PHYSICS_HEADER)
#include COMPUTED_INCLUDE(FILESYSTEM_HEADER)

class WinApplication : public Services
{
public:
    WinApplication();

    int Exec();

    static ITime *GetTime(const char *timeName = "Time", float timeScale = 1.0);

protected:
    bool Init();
};

#endif // WINAPPLICATION_H
