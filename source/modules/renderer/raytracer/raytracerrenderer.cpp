#include "gles2renderer.h"

bool GLES2Renderer::Init(
        bool fullscreen,
        const char *windowTitle,
        const unsigned int windowLength,
        const unsigned int windowHeight)
{
    bool success = gladLoadGL();

    if (success == false)
    {
        LogError("GLAD failed to load OpenGL");
        return false;
    }

    Log("OpenGL " + String(GLVersion.major) + "." + String(GLVersion.minor));

    if (GLVersion.major < 3)
    {
        LogError("OpenGL core profile failed to load");
        return false;
    }

    glGenVertexArrays(MAX_VAOS, vaos);
    glGenBuffers(MAX_BUFFERS, buffers);

    Resize(windowLength, windowHeight);

    //glEnable(GL_CULL_FACE);

    return true;
}

void GLES2Renderer::Resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLES2Renderer::Setup(GLES2Drawable *drawable)
{
    drawable->Activate();
}

void GLES2Renderer::Draw(IDrawable *drawable)
{
    glDrawArrays(GL_TRIANGLES, 0, drawable->vertices.size); // TODO: test with glDrawElements instead?
}

void GLES2Renderer::Render()
{
    PreRender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Test if it is better to sort by shader... although, simplicity usually does the trick
    for(int drawable = 0; drawable < drawables.size; drawable++)
    {
        Setup(drawables[drawable]); // First setup the object and an associated shader
        Draw(drawables[drawable]); // Draw object with shader
    }

    glFlush();
    GetError();

    PostRender();
}

IDrawable *GLES2Renderer::CreateDrawable(
        Array<IDrawable::Vertex> &vertices,
        Array<unsigned int> &indices,
        Array<String> &shaders,
        Array<IFile*> *textures)
{
    GLuint vaoID = GLuint(drawables.size + 1);
    GLuint vboVerticesID = vaoID;
    GLuint vboIndicesID  = vaoID + 1;

    glBindVertexArray(vaos[vaoID]);

    GLES2Drawable *drawable = new GLES2Drawable(vaoID, shaders, textures);

/*
    glBindBuffer(GL_ARRAY_BUFFER, buffers[vboVerticesID]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[vboIndicesID]);

    drawable->shader->Attribute(vboID_vertices, vertices, sizeof(vertices));
    drawable->shader->Element(vboID_indices, indices, sizeof(indices));

    drawables.Add(drawable);
*/
    return drawable;
}

void GLES2Renderer::RemoveDrawable(IDrawable *drawable)
{
    // TODO: Store vaoID and vboVerticesId and vboIndicesId of object i
    // Remove drawable at i
    for (int i = 0; i < drawables.size; i++)
    {
        if (drawables[i] == drawable)
        {
            drawables.RemoveAt(i);
            break;
        }
    }

    // TODO: Decriment vaoID and vboVerticesId and vboIndicesId
    // TODO: Set last drawable to have *Id of removed object
}

void GLES2Renderer::ClearDrawables()
{
    // TODO: Test...?
    drawables.Clear();
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
