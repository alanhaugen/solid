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

// For improving Sleep performance, we set a few constants here. See https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-sleep?redirectedfrom=MSDN
#define TARGET_RESOLUTION 1         // 1-millisecond target resolution

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

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
