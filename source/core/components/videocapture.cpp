#include "videocapture.h"
#include "core/application.h"

#ifdef THEORA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>

/* helper: push a page into the appropriate stream */
/* this can be done blindly; a stream won't accept a page
                that doesn't belong to it */

int VideoCapture::buffer_data(ogg_sync_state *oy){
    char *buffer=ogg_sync_buffer(oy, 4096);
    int bytes=fread(buffer, 1, 4096, infile);
    ogg_sync_wrote(oy, bytes);
    return(bytes);
}

int VideoCapture::queue_page(ogg_page *page)
{
    if(theora_p)ogg_stream_pagein(&to,page);
    return 0;
}
#endif

VideoCapture::VideoCapture()
{
    isDone = true;
#ifdef THEORA
    image = NULL;
    isDone = false;
    fps = NULL;
#endif
}

VideoCapture::VideoCapture(const String &filename)
{
    isDone = true;

#ifdef THEORA
    image = NULL;
    isDone = false;
    fps = NULL;
    //image = new Sprite(URL("data/logo_inv_jpg.jpg"));
    Open(filename);

    frameNum=0;
    skipNum=0;
#endif
}

VideoCapture::VideoCapture(int index)
{
    isDone = true;

#ifdef THEORA
    image = NULL;
    isDone = false;
    fps = NULL;
#endif
}

bool VideoCapture::Open(const String &filename)
{
#ifdef THEORA
    //IFile *video = filesystem->Open(filename);

    //delete video;

    fps = Application::GetTime();

    infile = fopen(filename.ToChar(), "rb");

    int i,j;
    int hasdatatobuffer = 1;
    int playbackdone = 0;
    double now, delay, last_frame_time = 0;

    int frameNum=0;
    int skipNum=0;

    /* start up Ogg stream synchronization layer */
    ogg_sync_init(&oy);

    /* init supporting Theora structures needed in header parsing */
    theora_comment_init(&tc);
    theora_info_init(&ti);

    int stateflag=0;

    while (!stateflag)
    {
        int ret = buffer_data(&oy);

        if (ret==0)
            break;

        while (ogg_sync_pageout(&oy,&og)>0)
        {
            ogg_stream_state test;

            /* is this a mandated initial header? If not, stop parsing */
            if(!ogg_page_bos(&og))
            {
                /* don't leak the page; get it into the appropriate stream */
                queue_page(&og);
                stateflag=1;
                break;
            }

            ogg_stream_init(&test,ogg_page_serialno(&og));
            ogg_stream_pagein(&test,&og);
            ogg_stream_packetout(&test,&op);

            /* identify the codec: try theora */
            if (!theora_p && theora_decode_header(&ti,&tc,&op)>=0)
            {
                /* it is theora */
                memcpy(&to,&test,sizeof(test));
                theora_p=1;
            }
            else
            {
                /* whatever it is, we don't care about it */
                ogg_stream_clear(&test);
            }
        }
    }

    /* we've now identified all the bitstreams. parse the secondary header packets. */
    while (theora_p && theora_p<3)
    {
        int ret;

        /* look for further theora headers */
        while (theora_p && (theora_p<3) && (ret=ogg_stream_packetout(&to,&op)))
        {
            if(ret<0)
            {
                LogError("Error parsing Theora stream headers; corrupt stream?\n");
                Release();
            }

            if (theora_decode_header(&ti,&tc,&op)){
                LogError("Error parsing Theora stream headers; corrupt stream?\n");
                Release();
            }

            theora_p++;

            if (theora_p==3)
                break;
        }

        /* The header pages/packets will arrive before anything else we
               care about, or the stream is not obeying spec */

        if(ogg_sync_pageout(&oy,&og)>0)
        {
            queue_page(&og); /* demux into the appropriate stream */
        }
        else
        {
            int ret=buffer_data(&oy);

            if(ret==0)
            {
                LogError("End of file while searching for codec headers.\n");
                Release();
            }
        }
    }

    if(theora_p){
        theora_decode_init(&td,&ti);
        //        Log("Ogg logical stream " + to.serialno + " is Theora " + ti.width + "x" + ti.height);//; + " " + (double)ti.fps_numerator/ti.fps_denominator + " fps video.");
        switch(ti.colorspace){
        case OC_CS_UNSPECIFIED:
            /* nothing to report */
            break;;
        case OC_CS_ITU_REC_470M:
            LogWarning("  encoder specified ITU Rec 470M color.");
            break;;
        case OC_CS_ITU_REC_470BG:
            LogWarning("  encoder specified ITU Rec 470BG color.");
            break;;
        default:
            LogWarning("warning: encoder specified unknown colorspace (%d).");
            break;
        }
        // dump_comments(&tc);
    }else{
        /* tear down the partial theora setup */
        theora_info_clear(&ti);
        theora_comment_clear(&tc);
    }

    int imageWidth = 0;
    int imageHeight = 0;

    if (theora_p)
    {
        imageWidth = ti.frame_width;
        imageHeight = ti.frame_height;
    }
#endif
    isDone = true; // HACK: TODO: REMOVE

    return true;
}

void VideoCapture::Release()
{
    isDone = true;
}

Sprite *VideoCapture::NextFrame()
{
#ifdef THEORA
    return image;
#else
    return NULL;
#endif
}

#ifdef THEORA
// Special thanks to David at the xiph boards: http://lists.xiph.org/pipermail/theora/2005-November/001033.html
#define clip(a) (((a)&256) ? ((a)>>31)^(-1) : (a))

void theoraToRGB(theora_info ti, yuv_buffer yuv, U8 *texture)
{
    // create yuvbuf in interleaved YUV format.

    int i = 0;
    int j = 0;
    int crop_offset = 0;

    unsigned char* yuvbuf = (unsigned char *)malloc(ti.width * ti.height * 3);
    //U8* rgbbuf = texture;//(char *)malloc(ti.width * ti.height * 3);

    crop_offset=ti.offset_x+yuv.y_stride*ti.offset_y;

    int k=0;
    i = 0;
    while(i<ti.height) {
        for(j=0;j<ti.width;j++) {
            yuvbuf[k]=yuv.y[crop_offset+yuv.y_stride*i+j];
            k += 3;
        }
        i++;
    }

    crop_offset=(ti.offset_x/2)+(yuv.uv_stride)*(ti.offset_y/2);

    k=1;
    i=0;
    while(i<ti.height/2) {
        for(j=0;j<ti.width/2;j++) {
            yuvbuf[k++]=yuv.u[crop_offset+yuv.uv_stride*i+j];
            yuvbuf[k++]=yuv.v[crop_offset+yuv.uv_stride*i+j];
            k++;
            yuvbuf[k++]=yuv.u[crop_offset+yuv.uv_stride*i+j];
            yuvbuf[k++]=yuv.v[crop_offset+yuv.uv_stride*i+j];
            k++;
        }
        for(j=0;j<ti.width/2;j++) {
            yuvbuf[k++]=yuv.u[crop_offset+yuv.uv_stride*i+j];
            yuvbuf[k++]=yuv.v[crop_offset+yuv.uv_stride*i+j];
            k++;
            yuvbuf[k++]=yuv.u[crop_offset+yuv.uv_stride*i+j];
            yuvbuf[k++]=yuv.v[crop_offset+yuv.uv_stride*i+j];
            k++;
        }
        i++;
    }

    // convert yuvbuf to rgbbuf, interleaved RGB.

    int offset = 0, y, u, v;

    while(offset < ti.width * ti.height * 3) {

        y = yuvbuf[offset++] - 16;
        u = yuvbuf[offset++] - 128;
        v = yuvbuf[offset++] - 128;

        offset -= 3;

        texture[offset++] = clip((298 * y + 409 * v + 128) >> 8);
        texture[offset++] = clip((298 * y + 100 * u - 208 * v + 128) >> 8);
        texture[offset++] = clip((298 * y + 516 * u + 128) >> 8);
    }

    free(yuvbuf);
}
#endif


void VideoCapture::Update()
{
#ifdef THEORA
    if (fps->TimeSinceStarted() >= (1000.0f / 60.0f))// (double)ti.fps_numerator/ti.fps_denominator))
    {
        fps->Reset();
        while(theora_p && !videobuf_ready){
            /* get one video packet... */
            if(ogg_stream_packetout(&to,&op)>0){

                theora_decode_packetin(&td,&op);

                videobuf_granulepos=td.granulepos;
                videobuf_time=theora_granule_time(&td,videobuf_granulepos);
                /* update the frame counter */
                frameNum++;

                /* check if this frame time has not passed yet.
                 If the frame is late we need to decode additonal
                 ones and keep looping, since theora at this stage
                 needs to decode all frames */
                //now=get_time();
                //delay=videobuf_time-now;
                //if(delay>=0.0){
                /* got a good frame, not late, ready to break out */
                videobuf_ready=1;
                //}else if(now-last_frame_time>=1.0){
                /* display at least one frame per second, regardless */
                videobuf_ready=1;
                //}else{
                //fprintf(stderr, "dropping frame %d (%.3fs behind)\n",
                //       frameNum, -delay);
                //}
            }else{
                /* need more data */
                break;
            }
        }

        if(!hasdatatobuffer && !videobuf_ready && !audiobuf_ready){
            isPlaying = 0;
            playbackdone = 1;
            isDone = true;
        }

        /* if we're set for the next frame, sleep */
        if(!theora_p || videobuf_ready) {
            //int ticks = 1.0e3*(videobuf_time-get_time());
            //if(ticks>0)
            //    SDL_Delay(ticks);
        }

        if(videobuf_ready){
            /* time to write our cached frame */
            //video_write();

            /* taken from player_sample.c test file for theora alpha */
            int i;
            yuv_buffer yuv;
            theora_decode_YUVout(&td,&yuv);

            /* Lock SDL_yuv_overlay */
            /*if ( SDL_MUSTLOCK(screen) ) {
                if ( SDL_LockSurface(screen) < 0 ) return;
            }
            if (SDL_LockYUVOverlay(yuv_overlay) < 0) return;
    */

            /* let's draw the data (*yuv[3]) on a SDL screen (*screen) */
            /* deal with border stride */
            /* reverse u and v for SDL */
            /* and crop input properly, respecting the encoded frame rect */
            //crop_offset=ti.offset_x+yuv.y_stride*ti.offset_y;

            int channels = 3;

            Pixmap *texture = new Pixmap(ti.frame_width *2, ti.frame_height); // (ti.frame_width *2) is a hack to fix video playback on win32 (TODO: figure out what is wrong)
            //        Pixmap *texture = new Pixmap(URL("data/logo_inv_jpg.jpg"));

            /*for(int j=0; j < height / 2; j++)
            {
                for(int i=0; i < width; i++)
                {
                    int idx = channels * (j*width + i);
                    //data[idx+c] = img[idx+c];
                    RGB colours = RGBfromYUV(yuv.y[yuv.uv_stride*j], yuv.u[yuv.uv_stride*j], yuv.v[yuv.uv_stride*j]);
                    texture->data[idx+0] = colours.R;
                    texture->data[idx+1] = colours.G;
                    texture->data[idx+2] = colours.B;
                }
            }*/

            theoraToRGB(ti, yuv, texture->data);

            /*        for (int i = 0; i < )
            memcpy(texture->data+i,
                        yuv.y+crop_offset+yuv.y_stride*i,
                        w);
    */
            // DISPLAY IMAGE WITH OPENGL!!!
            delete image;

            image = new Sprite(texture);

            delete texture;

            /*for(i=0;i<yuv_overlay->h;i++)
                memcpy(yuv_overlay->pixels[0]+yuv_overlay->pitches[0]*i,
                        yuv.y+crop_offset+yuv.y_stride*i,
                        yuv_overlay->w);
            crop_offset=(ti.offset_x/2)+(yuv.uv_stride)*(ti.offset_y/2);
            for(i=0;i<yuv_overlay->h/2;i++){
                memcpy(yuv_overlay->pixels[1]+yuv_overlay->pitches[1]*i,
                        yuv.v+crop_offset+yuv.uv_stride*i,
                        yuv_overlay->w/2);
                memcpy(yuv_overlay->pixels[2]+yuv_overlay->pitches[2]*i,
                        yuv.u+crop_offset+yuv.uv_stride*i,
                        yuv_overlay->w/2);
            }*/

            /* Unlock SDL_yuv_overlay */
            /*SDL_UnlockYUVOverlay(yuv_overlay);
            if ( SDL_MUSTLOCK(screen) ) {
                SDL_UnlockSurface(screen);
            }*/

            /* Show, baby, show! */
            //SDL_DisplayYUVOverlay(yuv_overlay, &rect);




            videobuf_ready=0;
            //last_frame_time=get_time();

        }

        /* HACK: always look for more audio data */
        audiobuf_ready=0;

        /* buffer compressed data every loop */
        if(hasdatatobuffer){
            hasdatatobuffer=buffer_data(&oy);
            /*if(hasdatatobuffer==0){
                }*/
        }

        if (ogg_sync_pageout(&oy,&og)>0){
            queue_page(&og);
        }
    }

    if (image && isDone == false)
    {
        image->Update();
    }
#endif
} /* playbackdone */

VideoCapture::~VideoCapture()
{
#ifdef THEORA
    Release();

    /* show number of video frames decoded */
    Log( "Frames decoded: " + String(frameNum) );
    //if(skipNum)
    //    printf( " (only %d shown)", frameNum-skipNum);
    //printf( "\n" );

    /* tear it all down */
    fclose( infile );

    if(theora_p){
        ogg_stream_clear(&to);
        theora_clear(&td);
        theora_comment_clear(&tc);
        theora_info_clear(&ti);
    }
    ogg_sync_clear(&oy);
#endif
}

void VideoCapture::UpdateAfterPhysics()
{
}
