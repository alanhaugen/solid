#ifndef SKIPCUTSCENE_H
#define SKIPCUTSCENE_H

#include "core/components/component.h"
#include "core/components/videocapture.h"

class SkipCutscene : public IComponent
{
private:
   VideoCapture *video;

public:
    SkipCutscene(VideoCapture *video_);

    void Update();
    void UpdateAfterPhysics();
};

#endif // SKIPCUTSCENE_H
