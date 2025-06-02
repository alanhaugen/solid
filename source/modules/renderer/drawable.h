#ifndef DRAWABLE_H
#define DRAWABLE_H

#define GLM_LEFT_HANDED // todo check if gles2 renderer is ok with this
#define _USE_MATH_DEFINES
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "core/containers/hash.h"
#include "core/containers/array.h"
#include "core/x-platform/string.h"
#include "core/x-platform/uniform.h"
#include "core/x-platform/ptr.h"

const unsigned int VERTEX_SHADER = 0;
const unsigned int FRAGMENT_SHADER = 1;

const unsigned int MAX_JOINTS = 120;

enum
{
    DRAW_TRIANGLES = 0,
    DRAW_LINES,
    CUBEMAP
};

class IDrawable
{
public:
    IDrawable()
        : colorTint(1.f)
    {
        type = DRAW_TRIANGLES;
        colorTint.x = 1.0f;
        visible = true;
        frame = 0;
        verticesPerFrameQuantity = 0;
        lastFrame = 0;
        hasDepth = true;
        sendToFront = false;
    }

    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
        glm::vec4 weights;
        glm::ivec4 joints;
        glm::i32 glyph;

        Vertex() { }
        Vertex(glm::vec3 p) { position = p; weights = glm::vec4(1.0f); glyph = -1; color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); }
        Vertex(glm::vec2 p) { position = glm::vec3(p, 0.0f); weights = glm::vec4(1.0f); glyph = -1; color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); }
        Vertex(glm::vec2 p, glm::i32 g) { position = glm::vec3(p, 0.0f); weights = glm::vec4(1.0f); glyph = g; }
        Vertex(glm::vec3 p, glm::vec4 c) { position = p; color = c; weights = glm::vec4(1.0f); glyph = -1; }
        Vertex(glm::vec3 p, glm::vec4 c, glm::vec2 texCoord) { position = p; color = c; textureCoordinates = texCoord; weights = glm::vec4(1.0f); glyph = -1; }
        Vertex(glm::vec3 p, glm::vec2 texCoord) { position = p; textureCoordinates = texCoord; weights = glm::vec4(1.0f); glyph = -1; }
    };

    int type;
    bool visible;
    bool draw;
    bool hasDepth;
    bool sendToFront;
    bool isTextured;

    glm::mat4 matrix; // Matrix is shorter to write than transform :)
    glm::vec4 colorTint; // Multiply by texture or vertex color
    glm::mat4 *animatedMatrices;
    unsigned int frame, verticesPerFrameQuantity, lastFrame;
    virtual ~IDrawable() {}

    struct UniformBlock
    {
        glm::mat4 MVP;	// combined modelview projection matrix
        glm::vec4 colour;
        glm::vec4 time;
        glm::vec4 index;
        glm::vec4 pos;
        glm::vec4 scaleX;
        glm::vec4 scaleY;
        glm::vec4 width;
        glm::vec4 height;
        glm::vec4 totalWidth;
        glm::vec4 totalHeight;
        glm::vec4 screenWidth;
        glm::vec4 screenHeight;
        glm::vec4 flip;
        glm::vec4 flipVertical;
        glm::vec4 colourTint;
        glm::mat4 modelMat;
        glm::mat4 normalMat;
        glm::vec4 lightPosition;
        glm::vec4 cameraPosition;
    };

    UniformBlock uniforms;
private:
};

#endif // DRAWABLE_H
