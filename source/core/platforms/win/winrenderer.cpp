#define WIN32_LEAN_AND_MEAN
#include "winrenderer.h"
#include <windows.h>
#include <tchar.h>

#include <glad/wgl.h>
#include <glad/gl.h>

WinRenderer::WinRenderer()
{
}
/*
WinRenderer::~WinRenderer() {
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(TRUE);

 // Clean-up:
    if (opengl_context)
        wglDeleteContext(opengl_context);
    if (hdc)
        ReleaseDC(hWnd, hdc);
    if (hWnd)
        DestroyWindow(hWnd);
}
*/
LRESULT CALLBACK MessageLoop(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_SIZE:
        Locator::renderer->Resize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_KEYDOWN:
        Locator::input.OnButton(wParam, true);
        break;

    case WM_KEYUP:
        Locator::input.OnButton(wParam, false);
        break;

    case WM_MOUSEMOVE:
        Locator::input.Mouse.dx = LOWORD(lParam) - Locator::input.Mouse.x;
        Locator::input.Mouse.dy = HIWORD(lParam) - Locator::input.Mouse.y;

        Locator::input.Mouse.x = LOWORD(lParam);
        Locator::input.Mouse.y = HIWORD(lParam);
        break;

    case WM_LBUTTONUP:
        Locator::input.Mouse.Up = true;
        Locator::input.Mouse.Down = false;
        Locator::input.Mouse.Released = true;
        break;

    case WM_LBUTTONDOWN:
        Locator::input.Mouse.Up = false;
        Locator::input.Mouse.Down = true;
        Locator::input.Mouse.Pressed = true;
        break;

    case WM_PAINT:
        break;

    case WM_SYSCOMMAND:                     // Intercept System Commands
    {
        switch (wParam) {                   // Check System Calls
            case SC_SCREENSAVE:             // Screensaver Trying To Start?
            case SC_MONITORPOWER:           // Monitor Trying To Enter Powersave?
            return 0;                       // Prevent From Happening
        }
        break;                              // Exit
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

bool WinRenderer::Init(bool fullscreen, const char *windowTitle, const unsigned int windowLength, const unsigned int windowHeight)
{
    WNDCLASSEX wcex;
    DWORD      dwExStyle;
    DWORD      dwStyle;

    hWnd = NULL;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    wcex.cbSize      = sizeof(wcex);
    wcex.style       = CS_OWNDC;
    wcex.lpfnWndProc = MessageLoop;
    wcex.cbClsExtra  = 0;
    wcex.cbWndExtra  = 0;
    wcex.hInstance   = hInstance;
    wcex.hIcon       = LoadIconA(hInstance, "id"); // large alt+tab icon
    wcex.hCursor     = LoadCursor(NULL, IDC_ARROW);

    wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = _T("Solid");
    wcex.hIconSm       = LoadIconA(hInstance, "id"); // small taskbar icon

    if (!RegisterClassEx(&wcex))                                    // Attempt To Register The Window Class
    {
        MessageBox(NULL,_T("Failed To Register The Window Class."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                           // Return FALSE
    }

    if (fullscreen)
    {
        DEVMODE dmScreenSettings;                   // Device Mode
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));       // Makes Sure Memory's Cleared
        dmScreenSettings.dmSize=sizeof(dmScreenSettings);       // Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth    = windowLength;            // Selected Screen Width
        dmScreenSettings.dmPelsHeight   = windowHeight;           // Selected Screen Height
        dmScreenSettings.dmBitsPerPel   = 32;             // Selected Bits Per Pixel
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
        {
            LogError("Failed to open in fullscreen mode");
            return false;
        }

        dwExStyle = WS_EX_APPWINDOW;              // Window Extended Style
        dwStyle = WS_POPUP;                       // Windows Style
        ShowCursor(FALSE);                      // Hide Mouse Pointer
    }
    else
    {
        dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; // Window Extended Style
        dwStyle=WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX; //| WS_MAXIMIZEBOX;               // Windows Style
    }

    hWnd = CreateWindowEx(
                dwExStyle, // EX Style
                wcex.lpszClassName, // lpClassName
                0, // window name
                dwStyle, // style
                CW_USEDEFAULT, // x
                CW_USEDEFAULT, // y
                windowLength, // length
                windowHeight, // height
                0, // hWndParent
                0, // hMenu
                hInstance, // hInstance
                0  // lpParam
                );

    if (hWnd == NULL) {
        LogError("Failed to create window");
        return false;
    }

    SetWindowTextA(hWnd, windowTitle);

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,            // The kind of framebuffer. RGBA or palette.
        32,                       // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                       // Number of bits for the depthbuffer
        8,                        // Number of bits for the stencilbuffer
        0,                        // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    ourWindowHandleToDeviceContext = GetDC(hWnd);

    int letWindowsChooseThisPixelFormat;
    letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
    SetPixelFormat(ourWindowHandleToDeviceContext,letWindowsChooseThisPixelFormat, &pfd);

    HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);

    if (ourWindowHandleToDeviceContext == NULL)
    {
        LogError("Window device context failed");
        return false;
    }

    if (wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext) == false)
    {
        LogError("Failed to attach rendering context");
        return false;
    }

    gladLoaderLoadWGL(ourWindowHandleToDeviceContext);

    // Set the desired OpenGL version:
    int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,   // Set the MAJOR version of OpenGL to 3
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,   // Set the MINOR version of OpenGL to 2
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,   // Demand OpenGL core profile
        0
    };

    // Create the final opengl context and get rid of the temporary one:
    HGLRC opengl_context = NULL;

    wglCreateContextAttribsARB_t aCreateCtxProc = (wglCreateContextAttribsARB_t)wglGetProcAddress("wglCreateContextAttribsARB"); // TODO: Figure out warning

    if (NULL == (opengl_context = aCreateCtxProc(ourWindowHandleToDeviceContext, NULL, attributes)))
    {
        wglDeleteContext(ourOpenGLRenderingContext);
        ReleaseDC(hWnd, ourWindowHandleToDeviceContext);
        DestroyWindow(hWnd);
        LogError("Failed to create the final rendering context!");

        return false;
    }

    wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
    wglDeleteContext(ourOpenGLRenderingContext); // Delete the temporary OpenGL context
    wglMakeCurrent(ourWindowHandleToDeviceContext, opengl_context);    // Make our OpenGL 3.2 context current

    // Load OpenGL
    if (!gladLoaderLoadGL())
    {
       wglMakeCurrent(NULL, NULL);
       wglDeleteContext(opengl_context);
       ReleaseDC(hWnd, ourWindowHandleToDeviceContext);
       DestroyWindow(hWnd);
       LogError("Glad Loader failed!");

       return false;
    }

    if (RENDERER::Init(fullscreen, windowTitle, windowLength, windowHeight) == false)
    {
        LogError("Renderer failed to initialize");
        return false;
    }

    // Set Window in middle of screen
    RECT rc;

    GetWindowRect (hWnd, &rc) ;
    xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right)  / 2;
    yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

    SetWindowPos(hWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    // Show window
    ShowWindow(hWnd, SW_SHOW);                  // Show The Window
    SetForegroundWindow(hWnd);                  // Slightly Higher Priority
    SetFocus(hWnd);                             // Sets Keyboard Focus To The Window

    Resize(windowLength, windowHeight);

    return true;
}

void WinRenderer::PostRender()
{
    SwapBuffers(ourWindowHandleToDeviceContext);
}

void WinRenderer::PreRender()
{

}
