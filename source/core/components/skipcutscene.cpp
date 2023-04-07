#include "skipcutscene.h"
#include "core/application.h"

SkipCutscene::SkipCutscene(VideoCapture *video_)
{
    video = video_;
}

void SkipCutscene::UpdateAfterPhysics()
{
}

void SkipCutscene::Update()
{
    if (video->isDone)
    {
        Application::NextScene();
    }
    else
    {
        for (unsigned int i = 0; i < KEYS_QUANTITY; i++)
        {
            if (input.Pressed(i))
            {
                video->Release();
            }
        }
    }
}
