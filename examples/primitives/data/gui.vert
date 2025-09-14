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
layout(location = 11) out float vTextureIndex;
#else
smooth out vec2 vSmoothTexcoord;

uniform vec2 i_pos;
uniform vec4 i_scaleX;
uniform vec4 i_scaleY;
uniform vec4 i_width;
uniform vec4 i_height;
uniform vec4 i_totalWidth;
uniform vec4 i_totalHeight;
uniform vec4 i_index;
uniform vec4 i_screenWidth;
uniform vec4 i_screenHeight;
uniform vec4 i_flip;
uniform vec4 i_flipVertical;
uniform vec4 i_time;
uniform vec4 i_colourTint;
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
#ifndef VULKAN
    float time = i_time.x;
    float index = i_index.x;
    vec2 pos = i_pos.xy;
    float scaleX = i_scaleX.x;
    float scaleY = i_scaleY.x;
    float width = i_width.x;
    float height = i_height.x;
    float totalWidth = i_totalWidth.x;
    float totalHeight = i_totalHeight.x;
    float screenWidth = i_screenWidth.x;
    float screenHeight = i_screenHeight.x;
    float flip = i_flip.x;
    float flipVertical = i_flipVertical.x;
    vec4 colourTint = i_colourTint;
#endif

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
    float textureIndex = uniformBuffer.index.y;
#endif
    float aspectRatio = float(screenWidth) / float(screenHeight);

    float w = float(width)  * float(scaleX);
    float h = float(height) * float(scaleY);

    // not sure if aspectRatio stuff makes any sense
#ifdef VULKAN
    float y = -vVertex.y; // Flip because of Vulkan's -Y axis
#else
    float y = vVertex.y;
#endif
    gl_Position = vec4((vVertex.x / float(screenWidth)) * w, (y / float(screenHeight)) * h, 0.0, 1.0);
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

#ifdef VULKAN
    vTextureIndex = textureIndex;
#endif
    //o_rotation = rotation;
}
