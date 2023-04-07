#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <stdio.h>
#include "core/x-platform/string.h"
#include "core/components/component.h"
#include "core/components/sprite.h"

#ifdef THEORA
#include <theora/theora.h>
#include <theora/theoradec.h>
#include <vorbis/codec.h>
#include "core/x-platform/pixmap.h"
#include "modules/renderer/drawable.h"
#endif

class Pixmap;
class VideoCapture : public IComponent
{
private:
#ifdef THEORA
    ogg_packet op;

    int hasdatatobuffer = 1;
    int isPlaying = 0;
    int playbackdone = 0;

    FILE * infile = NULL;

    int              theora_p=0;

    ogg_sync_state   oy;
    ogg_page         og;
    ogg_stream_state vo;
    ogg_stream_state to;
    theora_info      ti;
    theora_comment   tc;
    theora_state     td;

    int          videobuf_ready=0;
    ogg_int64_t  videobuf_granulepos=-1;
    double       videobuf_time=0;

    int          audiobuf_ready=0;
    ogg_int64_t  audiobuf_granulepos=0; /* time position of last sample */

    int frameNum;
    int skipNum;

    ITime *fps;
    Sprite *image;

    int buffer_data(ogg_sync_state *oy);
    int queue_page(ogg_page *page);
#endif
public:
    VideoCapture();
    VideoCapture(const String &filename);
    VideoCapture(int index);

    ~VideoCapture();

    bool Open(const String &filename);
    void Release();

    Sprite *NextFrame();

    bool isDone;

    void Update();
    void UpdateAfterPhysics();
};

#endif // VIDEOCAPTURE_H
