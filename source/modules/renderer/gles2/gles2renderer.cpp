#include "gles2renderer.h"
#include "core/x-platform/typedefs.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stbi_image_write.h" // TODO: remove, hack

bool GLES2Renderer::Init(
        bool fullscreen_,
        const char *windowTitle,
        const unsigned int windowLength,
        const unsigned int windowHeight)
{
    fullscreen = fullscreen_;
    NullRenderer::Init(fullscreen, windowTitle, windowLength, windowHeight);

    Resize(windowLength, windowHeight);

    //glEnable(GL_CULL_FACE);   // Backface culling

    glEnable(GL_DEPTH_TEST);    // Enables 3D
    glEnable(GL_SCISSOR_TEST);  // Enables ...
    glEnable(GL_BLEND);         // Enables alpha output in fragment shaders
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glLineWidth(10.0f); // NOTE: This is an invalid value for glLineWidth

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
    // windowWidth  = width;
    // windowHeight = height;

    // Recalculate boarders and scale of sprites
    borderWidth = 0;
    boarderHeight = 0;

    globalScaleWidth = windowWidth / width;
    globalScaleHeight = windowHeight / height;
}

void GLES2Renderer::renderView(const glm::mat4& projViewMatrix, glm::vec2 viewOffset, glm::vec2 viewSize)
{
    //glScissor(viewOffset.x, viewOffset.y, viewSize.x, viewSize.y); // TODO: Readd
    //glViewport(viewOffset.x, viewOffset.y, viewSize.x, viewSize.y);
    glScissor(0, 0, actualWindowWidth, actualWindowHeight);
    glViewport(0, 0, actualWindowWidth, actualWindowHeight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f,0.0f,0.0f,0.0f);

    // TODO: Test if it is better to sort by shader... although, simplicity usually does the trick
    //for(unsigned drawable = 0; drawable < drawables.Size(); drawable++)
    //{
    // NOTE: Remove please, I am going backwards through the array (slower!) to make sure new things are drawn first
    LinkedList<GLES2Drawable*>::Iterator drawable = drawables.End();

    for (; drawable != NULL; --drawable)
    {
        GLES2Drawable *gles2drawable = (*drawable);

        if (gles2drawable->draw)
        {
            gles2drawable->Activate(projViewMatrix);
            // Loop through and update lights
            //for (unsigned light = 0; light < lights.Size(); light++)
            //{
            //    lights[light].Activate(); // something like this ??
            //}

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
                if (gles2drawable->indicesQuantity > 0)
                {
                    glDrawElements(GL_LINES, gles2drawable->indicesQuantity, GL_UNSIGNED_INT, 0);
                }
                else
                {
                    glDrawArrays(GL_LINES, 0, gles2drawable->verticesQuantity);
                }
            }

            gles2drawable->DeActivate();
            gles2drawable->draw = false;
        }
    }
}

void GLES2Renderer::Render(const Array<glm::mat4>& projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray)
{
    PreRender();

    //assert( projViewMatrixArray.Size() == viewBoundsArray.Size() );

    for (unsigned i = 0; i<projViewMatrixArray.Size(); i++)
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

IDrawable *GLES2Renderer::CreateDrawable(Array<IDrawable::Vertex> &vertices,
        Array<unsigned int> &indices,
        Array<String> &shaders,
        Array<ITexture *> textures)
{
    GLES2Shader* shader = CreateShader(shaders);

    GLES2Drawable *drawable = new GLES2Drawable(vertices, indices, shader, textures);

    drawables.Append(drawable);

    return drawable;
}

IDrawable *GLES2Renderer::CreateDrawable(Array<IDrawable::Vertex> &vertices,
                                         Array<unsigned int> &indices,
                                         Array<String> &shaders,
                                         ITexture *texture)
{
    Array<ITexture *> textures;

    if (texture != NULL)
    {
        textures.Add(texture);
    }

    GLES2Shader* shader = CreateShader(shaders);

    GLES2Drawable* drawable = new GLES2Drawable(vertices, indices, shader, textures);

    drawables.Append(drawable);

    return drawable;
}

void GLES2Renderer::RemoveDrawable(IDrawable *drawable)
{
    LinkedList<GLES2Drawable*>::Iterator drawableIterator = drawables.Begin();

    for (int i = 0; drawableIterator != drawables.End(); ++drawableIterator)
    {
        GLES2Drawable *gles2drawable = (*drawableIterator);
        if (gles2drawable == drawable)
        {
            drawables.RemoveAt(i);
            delete gles2drawable;
            break;
        }

        i++;
    }
}

GLES2Texture *GLES2Renderer::FindTexture(String texturePath)
{
    for (unsigned int i = 0; i < textures.Size(); i++)
    {
        if (textures[i]->name == texturePath)
        {
            return textures[i];
        }
    }

    return NULL;
}

ITexture *GLES2Renderer::CreateTexture(int width, int height)
{
    return NULL;
}

ITexture *GLES2Renderer::CreateTexture(String filename)
{
    GLES2Texture *texture = FindTexture(filename);

    if (texture == NULL)
    {
        texture = new GLES2Texture(filename);

        textures.Add(texture);
    }

    return texture;
}

ITexture *GLES2Renderer::CreateTexture(String front, String back, String top, String bottom, String left, String right)
{
    GLES2Texture *texture = FindTexture(front);

    if (texture == NULL)
    {
        texture = new GLES2Texture(front, back, top, bottom, left, right);

        textures.Add(texture);
    }

    return texture;
}

void GLES2Renderer::RemoveTexture(ITexture *texture)
{

}

GLES2Shader* GLES2Renderer::CreateShader(Array<String> &shadersInput)
{
    GLES2Shader* shader;
    String shaderName = shadersInput[VERTEX_SHADER] + shadersInput[FRAGMENT_SHADER];

    // See if shader is already created before
    /*if (shaders.Empty() == false)
    {
        LinkedList<GLES2Shader*>::Iterator iterator = shaders.Begin();

        for (; (*iterator) != NULL; ++iterator)
        {
            if ((*iterator)->name == shaderName)
            {
                return (*iterator);
            }
        }
    }*/

    // Create a new shader
    shader = new GLES2Shader();

    shader->name = shaderName;
    shader->LoadGLSL(GL_VERTEX_SHADER, shadersInput[VERTEX_SHADER].ToChar());
    shader->LoadGLSL(GL_FRAGMENT_SHADER, shadersInput[FRAGMENT_SHADER].ToChar());

    shader->Compile();

    shaders.Append(shader);

    return shader;
}

void GLES2Renderer::ClearDrawables()
{
    /*LinkedList<GLES2Drawable*>::Iterator drawable = drawables.Begin();

    for (int i = 0; drawable != drawables.End(); ++drawable)
    {
        GLES2Drawable *gles2drawable = (*drawable);
        drawables.RemoveAt(i);
        delete gles2drawable;
        i++;
    }*/
}

void GLES2Renderer::ClearTextures()
{
    for (unsigned i = 0; i < textures.Size(); i++)
    {
        delete textures[i];
        textures[i] = NULL;
    }
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
    ClearTextures();
}
