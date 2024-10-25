#include "winapplication.h"
#include <mmsystem.h>
//#include <Synchapi.h>
#include <windows.h>

//#define TILTFIVE

#ifdef TILTFIVE
// Tilt5 setup
#include "tilt5/TiltFiveNative.h"
#include "glm/gtc/quaternion.hpp"

#define IDENTIFIER_BUFFER_SIZE 1024
#define GLASSES_BUFFER_SIZE    1024
#define PARAM_BUFFER_SIZE      1024
#define WAND_BUFFER_SIZE       4

#define SLEEP(ms) Sleep(ms)

// Get OpenVino
#include <openvino/openvino.hpp>

// Get DX3D
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <comdef.h> // for parsing win error messages

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context

ID3D11RenderTargetView *backbuffer;    // global declaration

ID3D11Resource* texture;
ID3D11Texture2D *texture2d;
ID3D11SamplerState* textureSamplerState;

// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //DXGI_FORMAT_R8G8B8A8_UINT;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &dev,
                                  NULL,
                                  &devcon);

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);

//    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);

    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = 1216;
    viewport.Height = 768;

    devcon->RSSetViewports(1, &viewport);

    // Fill backbuffer
    //devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(1.0f, 0.2f, 0.4f, 1.0f));
    //devcon->CopyResource(texture,);

/*
    texture = NULL;
    ID3D11Texture2D* pBuffer;

    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBuffer);

    if(texture == NULL)
    {
        D3D11_TEXTURE2D_DESC td;
        pBuffer->GetDesc(&td);
        dev->CreateTexture2D(&td, NULL, &texture);
    }*/

    swapchain->Present(0, 0);



    //devcon->CopyResource(texture,);

    //D3DX11CreateTextureFromFile(dev, "checker.png", NULL, NULL, &texture, NULL);
    //D3DX11SaveTextureToFile(devcon, texture, D3DX11_IFF_PNG, "save.png");

    /*D3D11_TEXTURE2D_DESC td;
    pBackBuffer->GetDesc(&td);

    D3D11_TEXTURE2D_DESC textureDesc;
                ZeroMemory(&textureDesc, sizeof(textureDesc));
                textureDesc.Width = 1216;
                textureDesc.Height = 768;
                textureDesc.MipLevels = 1;
                textureDesc.ArraySize = 1;
                textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT; // DXGI_FORMAT_R8G8B8A8_TYPELESS DXGI_FORMAT_R8G8B8A8_UNORM;
                textureDesc.SampleDesc.Count = 1;
                textureDesc.SampleDesc.Quality = 0;
                textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
                textureDesc.Usage = D3D11_USAGE_DEFAULT;

    dev->CreateTexture2D(&textureDesc, NULL, &texture2d);

    devcon->CopyResource(texture2d, pBackBuffer);*/

    pBackBuffer->Release();

    /*D3D11_TEXTURE2D_DESC fullScreenTextureDesc;
            ZeroMemory(&fullScreenTextureDesc, sizeof(fullScreenTextureDesc));
            fullScreenTextureDesc.Width = 1216;
            fullScreenTextureDesc.Height = 768;
            fullScreenTextureDesc.MipLevels = 1;
            fullScreenTextureDesc.ArraySize = 1;
            fullScreenTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            fullScreenTextureDesc.SampleDesc.Count = 1;
            fullScreenTextureDesc.SampleDesc.Quality = 0;
            fullScreenTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            fullScreenTextureDesc.Usage = D3D11_USAGE_DEFAULT;


    dev->CreateTexture2D(&fullScreenTextureDesc, nullptr, &texture2d);*/
/*
    D3D11_RESOURCE_DESC desc = {};
    desc.Width = 1216;
    desc.Height = 768;
    desc.MipLevels = 1;
    desc.DepthOrArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Flags = DXGI_FORMAT_R8G8B8A8_UINT; //D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    desc.Dimension = D3D11_RESOURCE_DIMENSION_TEXTURE2D;

    CD3DX11_HEAP_PROPERTIES defaultHeapProperties(D3D11_HEAP_TYPE_DEFAULT);

    dev->CreateCommittedResource(
        &defaultHeapProperties,
        D3D11_HEAP_FLAG_NONE,
        &desc,
        D3D11_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&texture)); // tex is type ID3D12Resource*
*/

    //Pixmap *tex = new Pixmap(640, 480);

    // Load texture
    //D3DX11_IMAGE_LOAD_INFO image_info;
    //image_info.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;

    // Load texture
    //hr = D3DX11CreateShaderResourceViewFromMemory(dev, tex->data, sizeof (U8) * 640*480*3, NULL, NULL, &texture, NULL);

    /*HRESULT hr = D3DX11CreateTextureFromFile(dev, "checker_black_white.png", NULL, NULL, &texture, NULL); // &image_info, NULL, &texture, NULL);

    if (FAILED(hr))
    {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();

        printf("Loading texture failed: %s", errMsg);
        LogError("Failed to load texture");
    }*/

    /*// Sampler (for shader)
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = dev->CreateSamplerState(&sampDesc, &textureSamplerState);*/
}


bool checkConnection(T5_Glasses glassesHandle, const char* id) {
    T5_ConnectionState connectionState;
    T5_Result err = t5GetGlassesConnectionState(glassesHandle, &connectionState);
    if (err) {
        printf("Glasses connection : '%s' -> Error: %s\n", id, t5GetResultMessage(err));
        return false;
    }

    switch (connectionState) {
    case kT5_ConnectionState_ExclusiveConnection:
        printf("Glasses connection : '%s' -> Connected\n", id);
        return true;

    case kT5_ConnectionState_ExclusiveReservation:
        printf("Glasses connection : '%s' -> Reserved\n", id);
        return true;

    case kT5_ConnectionState_NotExclusivelyConnected:
        printf("Glasses connection : '%s' -> Not Connected\n", id);
        return true;

    case kT5_ConnectionState_Disconnected:
        printf("Glasses connection : '%s' -> Device Lost\n", id);
        return true;

    default:
        printf("Glasses connection : '%s' -> Unexpected state!\n", id);
        return false;
    }
}

void findWands(T5_Glasses glassesHandle) {
    uint8_t count = WAND_BUFFER_SIZE;
    T5_WandHandle wandListBuffer[WAND_BUFFER_SIZE];

    T5_Result err = t5ListWandsForGlasses(glassesHandle, wandListBuffer, &count);
    if (err) {
        printf("Wand list      : Error : %s\n", t5GetResultMessage(err));
        return;
    }

    printf("Wand list      : %u wand(s)\n", count);
    for (int i = 0; i < count; i++) {
        printf(" Wand found    : ID=%d\n", wandListBuffer[i]);
    }

    // [ReadWand]
    if (count > 0) {
        // Enable wand stream
        T5_WandStreamConfig config;
        config.enabled = true;
        err            = t5ConfigureWandStreamForGlasses(glassesHandle, &config);
        if (err) {
            printf("Failed to enable stream : %s\n", t5GetResultMessage(err));
            return;
        }

        T5_WandStreamEvent event;
        for (int i = 0; i < 100; i++) {
            err = t5ReadWandStreamForGlasses(glassesHandle, &event, 100);
            if (err) {
                if (err == T5_TIMEOUT) {
                    continue;
                }

                printf("Failed to read stream : %s\n", t5GetResultMessage(err));
                return;
            }

            switch (event.type) {
            case kT5_WandStreamEventType_Connect:
                printf("WAND EVENT : CONNECT [%u]\n", event.wandId);
                break;

            case kT5_WandStreamEventType_Disconnect:
                printf("WAND EVENT : DISCONNECT [%u]\n", event.wandId);
                break;

            case kT5_WandStreamEventType_Desync:
                printf("WAND EVENT : DESYNC [%u]\n", event.wandId);
                break;

            case kT5_WandStreamEventType_Report:
                printf("WAND EVENT : REPORT [%u] %fx%f\n",
                       event.wandId,
                       event.report.stick.x,
                       event.report.stick.y);
                break;
            }
        }

        // Disable wand stream
        config.enabled = false;
        err            = t5ConfigureWandStreamForGlasses(glassesHandle, &config);
        if (err) {
            printf("Failed to disable stream : %s\n", t5GetResultMessage(err));
            return;
        }
    }
}

const char* gameboardTypeString(T5_GameboardType gameboardType) {
    switch (gameboardType) {
    case kT5_GameboardType_None:
        return "None";
    case kT5_GameboardType_LE:
        return "LE";
    case kT5_GameboardType_XE:
        return "XE";
    case kT5_GameboardType_XE_Raised:
        return "XE Raised";
    default:
        return "<Unknown>";
    }
}

// ExclusiveOps
void getPoses(T5_Glasses glassesHandle) {
    T5_Result err;
    T5_GlassesPose pose;

    printf("Getting some poses");

    bool waiting = false;
    for (int i = 0; i < 1000; i++) {
        err = t5GetGlassesPose(glassesHandle, &pose);
        if (err) {
            if (err == T5_ERROR_TRY_AGAIN) {
                if (!waiting) {
                    printf("\nWaiting...");
                    fflush(stdout);
                } else {
                    printf(".");
                    fflush(stdout);
                }
                SLEEP(100);
                i--;
                waiting = true;
                continue;
            } else {
                printf("Failed to get pose : %s\n", t5GetResultMessage(err));
                return;
            }
        }

        waiting = false;

        printf("\nGLASSES POSE : [%lu] (Board:%s) "
               "[%6.4f, %6.4f, %6.4f] [%6.4f, %6.4f, %6.4f, %6.4f]",
               pose.timestampNanos,
               gameboardTypeString(pose.gameboardType),
               pose.posGLS_GBD.x,
               pose.posGLS_GBD.y,
               pose.posGLS_GBD.z,
               pose.rotToGLS_GBD.w,
               pose.rotToGLS_GBD.x,
               pose.rotToGLS_GBD.y,
               pose.rotToGLS_GBD.z);

        SLEEP(16);
    }

    printf("\n");
}

// NonExclusiveOps
T5_Result endlesslyWatchSettings(T5_Glasses glassesHandle, const char* id) {
    T5_Result err;

    printf("Watching for changes to settings... (forever)\n");
    for (;;) {
        uint16_t count = PARAM_BUFFER_SIZE;
        T5_ParamGlasses paramBuffer[PARAM_BUFFER_SIZE];
        err = t5GetChangedGlassesParams(glassesHandle, paramBuffer, &count);
        if (err) {
            printf("Error getting changed params for '%s' : %s\n", id, t5GetResultMessage(err));
            return err;
        }

        // Get the values of the changed params
        for (int i = 0; i < count; i++) {
            switch (paramBuffer[i]) {
            case kT5_ParamGlasses_Float_IPD: {
                double value = 0.0;
                err          = t5GetGlassesFloatParam(glassesHandle, 0, paramBuffer[i], &value);
                if (err) {
                    printf("Error getting changed IPD for '%s' : %s\n",
                           id,
                           t5GetResultMessage(err));
                    return err;
                }

                printf("IPD changed for '%s' : %f\n", id, value);
            } break;

            case kT5_ParamGlasses_UTF8_FriendlyName: {
                char buffer[T5_MAX_STRING_PARAM_LEN];
                size_t bufferSize = T5_MAX_STRING_PARAM_LEN;
                err               = t5GetGlassesUtf8Param(
                            glassesHandle, paramBuffer[i], static_cast<T5_ParamGlasses>(0), buffer, &bufferSize);
                if (err) {
                    printf("Error getting changed friendly name for '%s' : %s\n",
                           id,
                           t5GetResultMessage(err));
                    return err;
                }

                printf("Friendly name changed for '%s' : '%s'\n", id, buffer);
            } break;

            default:
                // Ignore other parameters
                break;
            }
        }
    }
}

void connectGlasses(T5_Context t5ctx, const char* id) {
    // Create glasses
    T5_Glasses glassesHandle;
    T5_Result err = t5CreateGlasses(t5ctx, id, &glassesHandle);
    if (err) {
        printf("Error creating glasses '%s' : %s\n", id, t5GetResultMessage(err));
        return;
    } else {
        printf("Created glasses   : '%s'\n", id);
    }

    // CHECK CONNECTION STATE
    if (!checkConnection(glassesHandle, id)) {
        return;
    }

    // [Acquire]
    err = t5AcquireGlasses(glassesHandle, "Chess by Huldra");
    if (err) {
        printf("Error acquiring glasses '%s': %s\n", id, t5GetResultMessage(err));
        return;
    }

    printf("Glasses acquired      : '%s'\n", id);

    // CHECK CONNECTION STATE
    if (!checkConnection(glassesHandle, id)) {
        return;
    }

    // ENSURE GLASSES ARE READY
    for (;;) {
        err = t5EnsureGlassesReady(glassesHandle);
        if (err) {
            if (err == T5_ERROR_TRY_AGAIN) {
                // A non-sample program would probably sleep here or retry on another frame pass.
                continue;
            }
            printf("Error ensure glasses ready '%s': %s\n", id, t5GetResultMessage(err));
            return;
        }

        printf("Glasses ready     : '%s'\n", id);
        break;
    }
    // [EnsureReady]

    // CHECK CONNECTION STATE
    if (!checkConnection(glassesHandle, id)) {
        return;
    }

    // RECALL THE IDENTIFIER FOR GLASSES
    size_t bufferSize = IDENTIFIER_BUFFER_SIZE;
    char recalledId[IDENTIFIER_BUFFER_SIZE];
    err = t5GetGlassesIdentifier(glassesHandle, recalledId, &bufferSize);
    if (err) {
        printf("Error getting ID for glasses '%s' : %s\n", id, t5GetResultMessage(err));
        return;
    } else if (strcmp(id, recalledId) != 0) {
        printf("Mismatch getting ID for glasses : '%s' -> '%s'\n", id, recalledId);
    } else {
        printf("Got ID for glasses : '%s' -> '%s'\n", id, recalledId);
    }

    // BRIEFLY PERFORM AN EXCLUSIVE OPERATION ON THE GLASSES
    /*if (true) {
        getPoses(glassesHandle);
    }*/

    // FIND AND BRIEFLY WATCH CONNECTED WANDS
    /*if (true) {
        findWands(glassesHandle);
    }*/

    // CHECK CONNECTION STATE
    if (!checkConnection(glassesHandle, id)) {
        return;
    }

    printf("Glasses ready     : '%s'\n", id);

    // Enable this to watch for changes to the glasses settings
    if (false) {
        err = endlesslyWatchSettings(glassesHandle, id);
        if (err) {
            printf("Error watching settings for glasses '%s' : %s\n", id, t5GetResultMessage(err));
            return;
        }
    }

    /// [Release]
    // RELEASE THE GLASSES
    t5ReleaseGlasses(glassesHandle);
    /// [Release]

    // CHECK CONNECTION STATE
    if (!checkConnection(glassesHandle, id)) {
        return;
    }

    // FIND AND BRIEFLY WATCH CONNECTED WANDS
    // Note that the glasses are not acquired here!
    if (true) {
        findWands(glassesHandle);
    }

    // DESTROY THE GLASSES
    t5DestroyGlasses(&glassesHandle);
    printf("Destroyed glasses : '%s'\n", id);
}
#endif

WinApplication::WinApplication()
{
    SetDebug(new WinDebug());
    SetRenderer(new WinRenderer());
    SetAudio(new AUDIO());
    SetFileSystem(new FILESYSTEM());
    SetScript(new SCRIPT());
    SetPhysics(new PHYSICS());
}

ITime *WinApplication::GetTime(const char *timeName, float timeScale)
{
    return new WinTime(timeName, timeScale);
}

bool WinApplication::Init()
{
    Log("WinAPI Runtime");
    Log(String(__DATE__) + " " + String(__TIME__));

    return true;
}

int WinApplication::Exec()
{
    MSG msg;

    ITime *runtime = GetTime("Runtime");

#ifdef TILTFIVE
    bool glassesConnected = false;

    /// CreateContext
    T5_ClientInfo clientInfo = {
        .applicationId      = "com.Huldra.Chess",
        .applicationVersion = "1.0.0",
        .sdkType            = 0,
        .reserved           = 0,
    };

    T5_Context t5ctx;
    T5_Result err = t5CreateContext(&t5ctx, &clientInfo, 0);

    if (err) {
        LogError("Failed to create context");
    }

    // Now connect to tilt5 service (WaitForService)
    bool waiting = false;
    for (;;) {
        char serviceVersion[T5_MAX_STRING_PARAM_LEN];
        size_t bufferSize = T5_MAX_STRING_PARAM_LEN;
        err               = t5GetSystemUtf8Param(
                    t5ctx, kT5_ParamSys_UTF8_Service_Version, serviceVersion, &bufferSize);

        if (!err)
        {
            LogError("Service version : " + String(serviceVersion));
            break;
        }

        if (err == T5_ERROR_NO_SERVICE)
        {
            if (!waiting)
            {
                LogWarning("Waiting for service...\n");
                waiting = true;
            }
        }
        else
        {
            Log("Error getting service version : " + String(t5GetResultMessage(err)));
            break;
        }
    }

    // Check for glasses
    const char* id;

    for (;;) {
        size_t bufferSize = GLASSES_BUFFER_SIZE;
        char glassesListBuffer[GLASSES_BUFFER_SIZE];
        err = t5ListGlasses(t5ctx, glassesListBuffer, &bufferSize);
        if (!err) {
            int glassesCount = 0;

            const char* buffPtr = glassesListBuffer;
            for (;;) {
                // Get the length of the string and exit if we've read the
                // terminal string (Zero length)
                size_t stringLength = strnlen(buffPtr, GLASSES_BUFFER_SIZE);
                if (stringLength == 0) {
                    break;
                }

                fprintf(stderr, "Glasses : %s\n", buffPtr);
                glassesCount++;

                /// [WaitForGlasses1]
                connectGlasses(t5ctx, buffPtr);
                id = buffPtr;
                /// [WaitForGlasses3]

                // Advance through the returned values
                buffPtr += stringLength;
                if (buffPtr > (glassesListBuffer + GLASSES_BUFFER_SIZE)) {
                    LogWarning("Warning: list may be missing null terminator");
                    break;
                }
            }

            if (glassesCount == 0)
            {
                LogError("Tilt5 Glasses not found. Please make sure you connect your glasses to your PC");
            }
            else
            {
                glassesConnected = true;
            }

            Log("Listed glasses found: " + String(glassesCount));
            break;
        }

        if (err == T5_ERROR_NO_SERVICE) {
            if (!waiting) {
                Log("Waiting for service...");
                waiting = true;
            }
        } else {
            LogError("Error listing glasses : " + String(t5GetResultMessage(err)));
        }
    }

    // Connect to glasses and store handle

    T5_Glasses glassesHandle;

    err = t5CreateGlasses(t5ctx, id, &glassesHandle);

    if (err)
    {
        printf("Error creating glasses '%s' : %s\n", id, t5GetResultMessage(err));
    }
    else
    {
        printf("Created glasses   : '%s'\n", id);

        // CHECK CONNECTION STATE
        if (checkConnection(glassesHandle, id))
        {
            // Acquire
            err = t5AcquireGlasses(glassesHandle, "Chess by Huldra"); // NOTICE: Game name goes here

            if (err)
            {
                printf("Error acquiring glasses '%s': %s\n", id, t5GetResultMessage(err));
            }
            else
            {
                printf("Glasses acquired      : '%s'\n", id);

                // CHECK CONNECTION STATE
                if (checkConnection(glassesHandle, id)) {

                    // ENSURE GLASSES ARE READY
                    for (;;) {
                        err = t5EnsureGlassesReady(glassesHandle);
                        if (err) {
                            if (err == T5_ERROR_TRY_AGAIN) {
                                // A non-sample program would probably sleep here or retry on another frame pass.
                                continue;
                            }
                            LogError("Error ensure glasses ready : " + String(id) + String(t5GetResultMessage(err)));
                            //return -1;
                            break;
                        }

                        printf("Glasses ready     : '%s'\n", id);
                        break;
                    }

                    // CHECK CONNECTION STATE
                    if (checkConnection(glassesHandle, id))
                    {
                        glassesConnected = true;
                    }
                }
            }
        }
    }

    if (glassesConnected)
    {
        HWND hWnd = dynamic_cast<WinRenderer*>(renderer)->hWnd;

        InitD3D(hWnd);

        err = t5InitGlassesGraphicsContext(glassesHandle, kT5_GraphicsApi_D3d, dev); // kT5_GraphicsApi_Gl, NULL);

        if (err == T5_ERROR_INVALID_STATE)
        {
            LogError("Failed to make tilt5 graphics context: invalid state");
        }
        else if (err == T5_ERROR_INVALID_ARGS)
        {
            LogError("Failed to make tilt5 graphics context: invalid args");
        }
        else if (err != T5_SUCCESS)
        {
            LogError("Failed to make Tilt5 graphics context");
        }

        if (err == T5_SUCCESS)
        {
            Log("Glasses connected");
        }
    }

    ITime *poseTimer = GetTime("Time since last pose check");

    // Get OpenVino running
    // -------- Step 1. Initialize OpenVINO Runtime Core --------
    ov::Core core;

    // Load model
    std::shared_ptr<ov::Model> model = core.read_model("model.xml");

    // -------- Step 3. Configure preprocessing --------
    const ov::Layout tensor_layout{"NHWC"};

    ov::preprocess::PrePostProcessor ppp(model);
    // 1) input() with no args assumes a model has a single input
    ov::preprocess::InputInfo& input_info = ppp.input();
    // 2) Set input tensor information:
    // - precision of tensor is supposed to be 'u8'
    // - layout of data is 'NHWC'
    input_info.tensor().set_element_type(ov::element::u8).set_layout(tensor_layout);
    // 3) Here we suppose model has 'NCHW' layout for input
    input_info.model().set_layout("NCHW");
    // 4) output() with no args assumes a model has a single result
    // - output() with no args assumes a model has a single result
    // - precision of tensor is supposed to be 'f32'
    ppp.output().tensor().set_element_type(ov::element::f32);

    // 5) Once the build() method is called, the pre(post)processing steps
    // for layout and precision conversions are inserted automatically
    model = ppp.build();

    // -------- Step 4. read input images --------
    //slog::info << "Read input images" << slog::endl;

    ov::Shape input_shape = model->input().get_shape();
    const size_t width = input_shape[ov::layout::width_idx(tensor_layout)];
    const size_t height = input_shape[ov::layout::height_idx(tensor_layout)];
/*
    std::vector<std::shared_ptr<unsigned char>> images_data;
    std::vector<std::string> valid_image_names;
    for (const auto& i : image_names) {
        FormatReader::ReaderPtr reader(i.c_str());
        if (reader.get() == nullptr) {
            slog::warn << "Image " + i + " cannot be read!" << slog::endl;
            continue;
        }
        // Collect image data
        std::shared_ptr<unsigned char> data(reader->getData(width, height));
        if (data != nullptr) {
            images_data.push_back(data);
            valid_image_names.push_back(i);
        }
    }
    if (images_data.empty() || valid_image_names.empty())
        throw std::logic_error("Valid input images were not found!");

    // -------- Step 5. Loading model to the device --------
    // Setting batch size using image count
    const size_t batchSize = images_data.size();
    slog::info << "Set batch size " << std::to_string(batchSize) << slog::endl;
    ov::set_batch(model, batchSize);
    printInputAndOutputsInfo(*model);

    // -------- Step 6. Loading model to the device --------
    slog::info << "Loading model to the device " << FLAGS_d << slog::endl;
    ov::CompiledModel compiled_model = core.compile_model(model, FLAGS_d);

    // -------- Step 7. Create infer request --------
    slog::info << "Create infer request" << slog::endl;
    ov::InferRequest infer_request = compiled_model.create_infer_request();

    // -------- Step 8. Combine multiple input images as batch --------
    ov::Tensor input_tensor = infer_request.get_input_tensor();

    for (size_t image_id = 0; image_id < images_data.size(); ++image_id) {
        const size_t image_size = shape_size(model->input().get_shape()) / batchSize;
        std::memcpy(input_tensor.data<std::uint8_t>() + image_id * image_size,
                    images_data[image_id].get(),
                    image_size);
    }

    // -------- Step 9. Do asynchronous inference --------
    size_t num_iterations = 10;
    size_t cur_iteration = 0;
    std::condition_variable condVar;
    std::mutex mutex;
    std::exception_ptr exception_var;
    // -------- Step 10. Do asynchronous inference --------
    infer_request.set_callback([&](std::exception_ptr ex) {
        std::lock_guard<std::mutex> l(mutex);
        if (ex) {
            exception_var = ex;
            condVar.notify_all();
            return;
        }

        cur_iteration++;
        slog::info << "Completed " << cur_iteration << " async request execution" << slog::endl;
        if (cur_iteration < num_iterations) {
            // here a user can read output containing inference results and put new
            // input to repeat async request again
            infer_request.start_async();
        } else {
            // continue sample execution after last Asynchronous inference request
            // execution
            condVar.notify_one();
        }
    });

    // Start async request for the first time
    slog::info << "Start inference (asynchronous executions)" << slog::endl;
    infer_request.start_async();

    // Wait all iterations of the async request
    std::unique_lock<std::mutex> lock(mutex);
    condVar.wait(lock, [&] {
        if (exception_var) {
            std::rethrow_exception(exception_var);
        }

        return cur_iteration == num_iterations;
    });

    slog::info << "Completed async requests execution" << slog::endl;

    // -------- Step 11. Process output --------
    ov::Tensor output = infer_request.get_output_tensor();

    // Read labels from file (e.x. AlexNet.labels)
    std::string labelFileName = fileNameNoExt(FLAGS_m) + ".labels";
    std::vector<std::string> labels;

    std::ifstream inputFile;
    inputFile.open(labelFileName, std::ios::in);
    if (inputFile.is_open()) {
        std::string strLine;
        while (std::getline(inputFile, strLine)) {
            trim(strLine);
            labels.push_back(strLine);
        }
    }

    // Prints formatted classification results
    ClassificationResult classificationResult(output, valid_image_names, batchSize, N_TOP_RESULTS, labels);
    classificationResult.show();

    } catch (const std::exception& ex) {
        slog::err << ex.what() << slog::endl;
        return EXIT_FAILURE;
    } catch (...) {
    slog::err << "Unknown/internal exception happened." << slog::endl;
    return EXIT_FAILURE;*/
#endif

    // Increase timer resolution acceracy (for better Sleep functionality)
    TIMECAPS tc;
    UINT     wTimerRes;

    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
    {
        // Error; application can't continue.
        LogWarning("Failed to increase Sleep(ms) timer resolution! This will affect performance.");
    }

    wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);

    timeBeginPeriod(wTimerRes);

    // Main program loop
    while(isAlive)
    {
        // Take care of window and user input
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Calculate delta time
        deltaTime = time->TimeSinceStarted();
        float sleepMilliseconds = FRAME_TIME_MS - deltaTime;

        // Update framework
        UpdateServices();

#ifdef TILTFIVE
        if (glassesConnected)
        {
            input.Head.active = false;

            T5_GlassesPose pose;

            err = t5GetGlassesPose(glassesHandle, &pose);

            if (err == T5_SUCCESS)
            {
                input.Head.x = pose.posGLS_GBD.x;
                input.Head.y = pose.posGLS_GBD.y;
                input.Head.z = pose.posGLS_GBD.z;

                glm::quat rotation(pose.rotToGLS_GBD.w,
                                   pose.rotToGLS_GBD.x,
                                   pose.rotToGLS_GBD.y,
                                   pose.rotToGLS_GBD.z);

                //glm::toMat4(rotation)

                input.Head.yaw   = glm::yaw(rotation);
                input.Head.pitch = glm::pitch(rotation);
                input.Head.roll  = glm::roll(rotation);

                input.Head.active = true;
            }

            T5_FrameInfo frameInfo;

            frameInfo.vci.startY_VCI = -tan(glm::radians(60.0f) * 0.5f);
            frameInfo.vci.startX_VCI = frameInfo.vci.startY_VCI * 1280 / (float)720;
            frameInfo.vci.width_VCI  = -2.0f * frameInfo.vci.startX_VCI;
            frameInfo.vci.height_VCI = -2.0f * frameInfo.vci.startY_VCI;

            frameInfo.texWidth_PIX  = 1216;
            frameInfo.texHeight_PIX = 768;

            //D3DX11CreateTextureFromFile(dev, "tga_file.tga", NULL, NULL, &texture, NULL);

            HRESULT hr = D3DX11CreateTextureFromFile(dev, "out.png", NULL, NULL, &texture, NULL);

            if (FAILED(hr))
            {
                LogError("Failed to load texture from filesystem");
            }

            /*D3DX11_IMAGE_INFO imgInfo;
            HRESULT result  = D3DX11GetImageInfoFromMemory(renderer->framebufferdata, 3 * 1216 * 768, NULL, &imgInfo, NULL);

            if (FAILED(result))
            {
                LogError("Failed to load from memory");
            }

            loadInfo.pSrcInfo = &imgInfo;*/

            //Get image info
            /*D3DX11_IMAGE_INFO imgInfo;

            HRESULT hr = D3DX11GetImageInfoFromMemory(
                        (LPCSTR)renderer->framebufferdata,
                        3 * 1216 * 768,
                        NULL,
                        &imgInfo,
                        NULL);

            if (hr)
            {
                LogError("Failed to make image info struct");
            }

            D3DX11_IMAGE_LOAD_INFO loadImgInfo;
            loadImgInfo.MipLevels = 0;//bMipmap ? 0 : 1;
            loadImgInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            loadImgInfo.Usage = D3D11_USAGE_STAGING;
            loadImgInfo.BindFlags = 0;// (autogen) ? (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET) : (D3D11_BIND_SHADER_RESOURCE);
            loadImgInfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
            loadImgInfo.MiscFlags = 0;

            hr = D3DX11CreateTextureFromMemory(dev, renderer->framebufferdata, 3 * 1216 * 768, /*&loadImgInfo, NULL, &texture, NULL);

            if (FAILED(hr))
            {
                LogError("Failed to make texture out of framebuffer");
            }*/

            /*HRESULT hr = D3DX11CreateTextureFromMemory(dev, renderer->framebufferdata, sizeof(renderer->framebufferdata), NULL, NULL, &texture, NULL);

            if (FAILED(hr))
            {
                LogError("Failed to make texture out of framebuffer");
            }*/

            //            CreateTGATextureFromFile();
            frameInfo.leftTexHandle  = texture; //leftEyeDisplay.texture->HandleAsVoidPtr();
            frameInfo.rightTexHandle = texture; //rightEyeDisplay.texture->HandleAsVoidPtr();

            frameInfo.posLVC_GBD = pose.posGLS_GBD;
            frameInfo.posRVC_GBD = pose.posGLS_GBD;

            frameInfo.rotToLVC_GBD = pose.rotToGLS_GBD;
            frameInfo.rotToRVC_GBD = pose.rotToGLS_GBD;

            frameInfo.isUpsideDown = false;
            frameInfo.isSrgb = true;

            //old
            /*    T5_FrameInfo *info = new T5_FrameInfo();

            info->texWidth_PIX = 640;
            info->texHeight_PIX = 480;

            info->leftTexHandle = texture;
            info->rightTexHandle = texture;
            info->isSrgb = false;
            info->isUpsideDown = false;

            info->vci.startX_VCI = 0;
            info->vci.startY_VCI = 0;
            info->vci.width_VCI = 0;
            info->vci.height_VCI = 0;

            info->posLVC_GBD = pose.posGLS_GBD;
            info->posRVC_GBD = pose.posGLS_GBD;

            info->rotToLVC_GBD = pose.rotToGLS_GBD;
            info->rotToRVC_GBD = pose.rotToGLS_GBD;*/

            //err = t5SendFrameToGlasses(glassesHandle, &frameInfo); // TODO: bring back
            err = T5_SUCCESS;

            //delete info;

            if (err == T5_ERROR_INVALID_ARGS)
            {
                LogError("Failed to send graphics to glasses: invalid args");
            }
            else if (err == T5_ERROR_INVALID_STATE)
            {
                LogError("Failed to send graphics to glasses: invalid state");
            }
            else if (err == T5_ERROR_NOT_CONNECTED)
            {
                LogError("Failed to send graphics to glasses: no connection");
            }
            else if (err == T5_ERROR_INVALID_GFX_CONTEXT)
            {
                LogError("Failed to send graphics to glasses: no gfx context");
            }
            else if (err == T5_ERROR_GFX_CONTEXT_INIT_FAIL)
            {
                LogError("Failed to send graphics to glasses: gfx context init failed");
            }
            else if (err == T5_ERROR_NO_CONTEXT)
            {
                LogError("Failed to send graphics to glasses: no context");
            }

            if (err == T5_SUCCESS)
            {
                //LogError("success");
            }
            if (err != T5_SUCCESS)
            {
                LogError("Image transfer failed for unspecified reason");
            }
        }
#endif

        // This will be set to true if it mouse was released this frame
        input.Mouse.Released = false;
        input.Mouse.Pressed  = false;
        input.Mouse.dx = 0;
        input.Mouse.dy = 0;

        // Centre mouse if requested to allow dx and dy to be measured continously
        if (input.Mouse.Dragging)
        {
            WinRenderer *winRenderer = dynamic_cast<WinRenderer*>(renderer);

            int x = winRenderer->xPos + (winRenderer->windowWidth / 2);
            int y = winRenderer->yPos + (winRenderer->windowHeight / 2);

            input.Mouse.x = (winRenderer->windowWidth / 2) - 3; // -3 is a magic number... not sure why it drifts without it
            input.Mouse.y = (winRenderer->windowHeight / 2) - 26; // -26 is a magic number

            SetCursorPos(x, y);
        }

        // Hide mouse if requested
        if (input.Mouse.Hidden)
        {
            ShowCursor(FALSE);
        }
        else
        {
            ShowCursor(TRUE);
        }

        // Let the operating system take over
        if (sleepMilliseconds > 0.0f)
        {
            Sleep(int(sleepMilliseconds));
            time->Reset();
        }
    }

    delete runtime;

    // Reset clock back to normal
    timeEndPeriod(wTimerRes);

#ifdef TILTFIVE
    delete poseTimer;

    if (glassesConnected)
    {
        t5DestroyGlasses(&glassesHandle);
    }

    t5DestroyContext(&t5ctx);

    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
    texture->Release();
#endif

    return msg.wParam;
}
