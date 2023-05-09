#include "gles2renderer.h"
#include "core/x-platform/typedefs.h"
#include "core/x-platform/pixmap.h"


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stbi_image_write.h" // TODO: remove, hack


bool GLES2Renderer::Init(
        bool fullscreen,
        const char *windowTitle,
        const unsigned int windowLength,
        const unsigned int windowHeight)
{
    UNUSED(fullscreen);
    UNUSED(windowTitle);
    Resize(windowLength, windowHeight);

    //glEnable(GL_CULL_FACE);   // Backface culling
    glEnable(GL_DEPTH_TEST);    // Enables 3D
    glEnable(GL_SCISSOR_TEST);  // Enables ...
    glEnable(GL_BLEND);         // Enables alpha output in fragment shaders
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(10.0f);

    framebufferdata = NULL;

    return true;
}

void GLES2Renderer::DrawDebugText(float x, float y, const char* str)
{
    UNUSED(x);
    UNUSED(y);
    UNUSED(str);
}

void GLES2Renderer::Resize(int width, int height)
{
    //Log(String("HW Window Resize Event to " + String(width)+"x"+String(height)));
    windowWidth = width;
    windowHeight = height;
}

void GLES2Renderer::renderView(const glm::mat4& projViewMatrix, glm::vec2 viewOffset, glm::vec2 viewSize)
{
    glScissor(viewOffset.x, viewOffset.y, viewSize.x, viewSize.y);
    glViewport(viewOffset.x, viewOffset.y, viewSize.x, viewSize.y);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f,0.0f,0.0f,0.0f);

    // TODO: Test if it is better to sort by shader... although, simplicity usually does the trick
    for(unsigned drawable = 0; drawable < drawables.Size(); drawable++)
    {
        GLES2Drawable *gles2drawable = drawables[drawable];
        if (gles2drawable->draw)
        {
            gles2drawable->Activate(projViewMatrix);
            /*// Loop through and update lights
            for (unsigned light = 0; light < lights.Size(); light++)
            {
                lights[light].Activate(); // something like this ??
            }*/

            // Render in 3D
            if (gles2drawable->hasDepth == true)
            {
                glDepthMask(GL_TRUE);
                glDepthRange(0, 1.00);
            }
            // Render on either on top of everything else or behind everything else
            else
            {
                if (gles2drawable->sendToFront)
                {
                    glDepthMask(GL_TRUE);
                    glDepthRange(0, 0.01);
                }
                // Make sure the stuff you want to render on top of everything else is rendered last
                // and the stuff you want at the back (like backgrounds) are rendered first!
                else
                {
                    glDepthMask(GL_FALSE);
                    glDepthRange(0, 1.00);
                }
            }

            if (gles2drawable->type == DRAW_TRIANGLES)
            {
                if (gles2drawable->indicesQuantity > 0)
                {
                    glDrawElements(GL_TRIANGLES, gles2drawable->indicesQuantity, GL_UNSIGNED_INT, 0);
                }
                else
                {
                    glDrawArrays(GL_TRIANGLES, 0, gles2drawable->verticesQuantity);
                }
            }
            else if (gles2drawable->type == DRAW_LINES)
            {
                glDrawElements(GL_LINES, gles2drawable->indicesQuantity, GL_UNSIGNED_INT, 0);
            }

            gles2drawable->DeActivate();
            gles2drawable->draw = false;
        }
    }
}

void GLES2Renderer::Render(const Array<glm::mat4>& projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray)
{
    PreRender();

    assert( projViewMatrixArray.Size() == viewBoundsArray.Size() );

    for(unsigned i = 0; i<projViewMatrixArray.Size(); i++)
    {
        const glm::vec4& bounds = viewBoundsArray[i];
        glm::vec2 off = glm::vec2( bounds.x, bounds.y );
        glm::vec2 size = glm::vec2( bounds.z, bounds.w );
        renderView(projViewMatrixArray[i], off, size);
    }

    glFlush();
    GetError();



    // TODO: remove, hack
    //short  TGAhead[] = {0, 2, 0, 0, 0, 0, windowWidth, windowHeight, 24};
    /*delete framebufferdata;
    framebufferdata = new int[windowWidth * windowHeight * 3];
    glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR, GL_UNSIGNED_BYTE, framebufferdata);

    stbi_flip_vertically_on_write(true);
    stbi_write_png("out.png", windowWidth, windowHeight, 3, framebufferdata, windowWidth * 3);*/

    //framebufferdata = new int[(windowWidth * windowHeight * 3) + sizeof(TGAhead)];
    //memcpy(memcpy(framebufferdata, TGAhead, sizeof (TGAhead)), buffer, windowWidth * windowHeight * 3); // append together
    //delete buffer;

    //FILE   *out = fmemopen(framebufferdata, (windowWidth * windowHeight * 3) + sizeof(TGAhead), "w");
    //fwrite(&TGAhead, sizeof(TGAhead), 1, out);
    //fwrite(buffer, 3 * windowWidth * windowHeight, 1, out);
    //delete buffer;
    //fmemclose(out);

    PostRender();
}

IDrawable *GLES2Renderer::CreateDrawable(
        Array<IDrawable::Vertex> &vertices,
        Array<unsigned int> &indices,
        Array<String> &shaders,
        Array<Pixmap *> *textures)
{
    GLES2Drawable *drawable = new GLES2Drawable(vertices, indices, shaders, textures);

    drawables.Add(drawable);

    return drawable;
}

void GLES2Renderer::RemoveDrawable(IDrawable *drawable)
{
    for (unsigned i = 0; i < drawables.Size(); i++)
    {
        if (drawables[i] == drawable)
        {
            delete drawables[i];
            drawables.RemoveAt(i);
            break;
        }
    }
}

void GLES2Renderer::ClearDrawables()
{
    /*for (unsigned i = 0; i < drawables.Size(); i++)
    {
        delete drawables[i];
        drawables[i] = 0x0;
    }*/
}

void GLES2Renderer::GetError()
{
    //! https://www.khronos.org/opengl/wiki/OpenGL_Error
    switch(glGetError())
    {
    case GL_NO_ERROR:
        break;

    case GL_INVALID_ENUM:
        LogWarning("GL invalid enum. "
                   "Given when an enumeration parameter "
                   "is not a legal enumeration for that "
                   "function. This is given only for "
                   "local problems; if the spec allows "
                   "the enumeration in certain circumstances, "
                   "where other parameters or state dictate "
                   "those circumstances, then "
                   "GL_INVALID_OPERATION is the result instead.");
        break;

    case GL_INVALID_VALUE:
        LogWarning("GL invalid value. "
                   "Given when a value parameter is not a "
                   "legal value for that function. This is only "
                   "given for local problems; if the spec allows "
                   "the value in certain circumstances, where "
                   "other parameters or state dictate those "
                   "circumstances, then GL_INVALID_OPERATION "
                   "is the result instead.");
        break;

    case GL_INVALID_OPERATION:
        LogWarning("GL invalid operation. "
                   "Given when the set of state for a command "
                   "is not legal for the parameters given to "
                   "that command. It is also given for commands "
                   "where combinations of parameters define what "
                   "the legal parameters are.");
        break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        LogWarning("GL invalid framebuffer operation. "
                   "Given when doing anything that would attempt "
                   "to read from or write/render to a framebuffer "
                   "that is not complete.");
        break;

    case GL_OUT_OF_MEMORY:
        LogWarning("GL out of memory. "
                   "Given when performing an operation that can "
                   "allocate memory, and the memory cannot be "
                   "allocated. The results of OpenGL functions "
                   "that return this error are undefined; it is "
                   "allowable for partial operations to happen.");
        break;

    /*case GL_STACK_UNDERFLOW:
        LogWarning("GL stack underflow.");
        break;

    case GL_STACK_OVERFLOW:
        LogWarning("GL stack overflow.");
        break;*/

    default:
        LogWarning("Unkown OpenGL error.");
    }
}

GLES2Renderer::~GLES2Renderer()
{
    ClearDrawables();
}
