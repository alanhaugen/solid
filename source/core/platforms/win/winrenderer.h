#ifndef WINRENDER_H
#define WINRENDER_H

#include "wintypes.h"
#include "winapplication.h"
#include "core/x-platform/computedinclude.h"
#include COMPUTED_INCLUDE(RENDERER_HEADER)

#define GL_SHADING_LANGUAGE_VERSION 0x8B8C

// WGL_ARB_pixel_format
#define WGL_DRAW_TO_WINDOW_ARB  0x2001
#define WGL_DRAW_TO_BITMAP_ARB  0x2002
#define WGL_ACCELERATION_ARB    0x2003
#define WGL_SUPPORT_GDI_ARB     0x200F
#define WGL_SUPPORT_OPENGL_ARB  0x2010
#define WGL_DOUBLE_BUFFER_ARB   0x2011
#define WGL_STEREO_ARB          0x2012
#define WGL_PIXEL_TYPE_ARB      0x2013
#define WGL_COLOR_BITS_ARB      0x2014
#define WGL_DEPTH_BITS_ARB      0x2022
#define WGL_STENCIL_BITS_ARB    0x2023

#define WGL_NO_ACCELERATION_ARB      0x2025
#define WGL_GENERIC_ACCELERATION_ARB 0x2026
#define WGL_FULL_ACCELERATION_ARB    0x2027

#define WGL_TYPE_RGBA_ARB       0x202B
#define WGL_TYPE_COLORINDEX_ARB 0x202C

// WGL_ARB_create_context_profile
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB         0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB  0x9126

// WGL_CONTEXT_FLAGS bits
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002

// WGL_CONTEXT_PROFILE_MASK_ARB bits
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define GL_NUM_EXTENSIONS 0x821D

typedef HGLRC (WINAPI *wglCreateContextAttribsARB_t)(HDC theDevCtx, HGLRC theShareContext, const int* theAttribs);

class WinRenderer : public RENDERER
{
public:
    WinRenderer();
    //~WinRenderer();

    bool Init(bool fullscreen, const char *windowTitle, const unsigned int windowLength, const unsigned int windowHeight);

    void PreRender();

    void PostRender();

    int xPos, yPos;

    HWND hWnd;

private:
    HDC ourWindowHandleToDeviceContext;
};

#endif // WINRENDER_H
