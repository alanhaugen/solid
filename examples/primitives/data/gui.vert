#version 330 core
// es
//core

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

// Thanks to Gerdelan, Anton. Anton's OpenGL 4 Tutorials (p. 364). Kindle Edition.

layout(location = 0) in vec3 vVertex;	//object space vertex position
layout(location = 1) in vec4 vColor;	//per-vertex colour
layout(location = 2) in vec4 vNormal;	//per-vertex normals
layout(location = 3) in vec2 vTexcoord;	//per-vertex texcoord
layout(location = 6) in int  vGlyph;	//glyph per vertex

#ifdef VULKAN
layout(set = 0, binding = 0) uniform UniformBlock
{
  mat4 MVP;	// combined modelview projection matrix
  vec4 colour;
  vec4 time;
  vec4 index;
  vec4 pos;
  vec4 scaleX;
  vec4 scaleY;
  vec4 width;
  vec4 height;
  vec4 totalWidth;
  vec4 totalHeight;
  vec4 screenWidth;
  vec4 screenHeight;
  vec4 flip;
  vec4 flipVertical;
  vec4 colourTint;
  mat4 modelMat;
  mat4 normalMat;
  vec4 lightPosition;
  vec4 cameraPosition;
} uniformBuffer;

layout(location = 0) out vec4 vSmoothColor;		//smooth colour to fragment shader
layout(location = 1) out vec2 vSmoothTexcoord;
layout(location = 2) out float vTime;
layout(location = 3) out float vIndex;
layout(location = 4) out float vWidth;
layout(location = 5) out float vHeight;
layout(location = 6) out float vTotalwidth;
layout(location = 7) out float vTotalheight;
layout(location = 8) out float vFlip;
layout(location = 9) out float vFlipVertical;
layout(location = 10) out vec4 vColourTint;
#else
smooth out vec2 vSmoothTexcoord;

uniform vec2 pos;
uniform float scaleX;
uniform float scaleY;
uniform float width;
uniform float height;
uniform float totalWidth;
uniform float totalHeight;
uniform float index;
uniform float screenWidth;
uniform float screenHeight;
uniform float flip;
uniform float flipVertical;
uniform float time;
uniform vec4 colourTint;
//uniform vec2 rotation;

out float vIndex;
out float vWidth;
out float vHeight;
out float vTotalwidth;
out float vTotalheight;
out float vFlip;
out float vFlipVertical;
out float vTime;
out vec4 vColourTint;
//out vec2 o_rotation;
#endif

void main()
{
#ifdef VULKAN
    vec4 colour = uniformBuffer.colour;
    mat4 MVP = uniformBuffer.MVP;
    float time = uniformBuffer.time.x;
    float index = uniformBuffer.index.x;
    vec2 pos = uniformBuffer.pos.xy;
    float scaleX = uniformBuffer.scaleX.x;
    float scaleY = uniformBuffer.scaleY.x;
    float width = uniformBuffer.width.x;
    float height = uniformBuffer.height.x;
    float totalWidth = uniformBuffer.totalWidth.x;
    float totalHeight = uniformBuffer.totalHeight.x;
    float screenWidth = uniformBuffer.screenWidth.x;
    float screenHeight = uniformBuffer.screenHeight.x;
    float flip = uniformBuffer.flip.x;
    float flipVertical = uniformBuffer.flipVertical.x;
    vec4 colourTint = uniformBuffer.colourTint;
#endif
    float aspectRatio = float(screenWidth) / float(screenHeight);

    float w = float(width)  * float(scaleX);
    float h = float(height) * float(scaleY);

    // not sure if aspectRatio stuff makes any sense
    gl_Position = vec4((vVertex.x / float(screenWidth)) * w, (vVertex.y / float(screenHeight)) * h, 0.0, 1.0);
    vSmoothTexcoord = vTexcoord;

    float halfScreenWidth  = float(screenWidth)  / 2.0f;
    float halfScreenHeight = float(screenHeight) / 2.0f;

    gl_Position.x += -1.0 + (float(width  * scaleX) / 2.0f) / halfScreenWidth   + pos.x / (halfScreenWidth);
    gl_Position.y -= -1.0 + (float(height * scaleY) / 2.0f) / halfScreenHeight  + pos.y / (halfScreenHeight);// - 11)); // 11 is the top of a window on windows
    //gl_Position.y += time;

    //gl_Position.x = gl_Position.x * rotation.y + gl_Position.y * rotation.x;
    //gl_Position.y = gl_Position.y * rotation.x + gl_Position.x * rotation.y;

    if (vGlyph == -1)
    {
        vIndex = float(index);
    }
    else
    {
        vIndex = float(vGlyph);
    }

    vWidth = float(width);
    vHeight = float(height);
    vTotalwidth = float(totalWidth);
    vTotalheight = float(totalHeight);
    vFlip = float(flip);
    vFlipVertical = float(flipVertical);
    vTime = float(time);
    vColourTint = colourTint;
    //o_rotation = rotation;
}
