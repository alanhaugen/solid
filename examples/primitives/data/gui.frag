#version 330 core
// es
//core

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

//layout(location=0)

#ifdef VULKAN
//#extension GL_EXT_nonuniform_qualifier : require
//input form the vertex shader
layout(location = 0) out vec4 vFragColor;	//fragment shader output
layout(set=1, binding=0) uniform sampler2D Combined[];
layout(set=1, binding=0) uniform sampler2D textureSampler;

layout(location = 0) in vec4 vSmoothColor;		//smooth colour to fragment shader
layout(location = 1) in vec2 vSmoothTexcoord;
layout(location = 2) in float vTime;
layout(location = 3) in float vIndex;
layout(location = 4) in float vWidth;
layout(location = 5) in float vHeight;
layout(location = 6) in float vTotalwidth;
layout(location = 7) in float vTotalheight;
layout(location = 8) in float vFlip;
layout(location = 9) in float vFlipVertical;
layout(location = 10) in vec4 vColourTint;
layout(location = 11) in float vTextureIndex;
#else
out vec4 vFragColor;	//fragment shader output

//input form the vertex shader
smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader
smooth in vec2 vSmoothTexcoord;
uniform sampler2D textureSampler;

in float vIndex;
in float vWidth;
in float vHeight;
in float vTotalwidth;
in float vTotalheight;
in float vFlip;
in float vFlipVertical;
in float vTime;
in vec4 vColourTint;
#endif
//in vec2 o_rotation;

void main ()
{
    vec4 final;

    vec2 coords = vSmoothTexcoord;

    // Calculate what area of the spritesheet to use
    // (or use entire sheet if width and height are equal to total width and height)
    float x, y, sum;
    x = coords.x * (vWidth  / vTotalwidth) + vIndex * (vWidth  / vTotalwidth);
    y = coords.y * (vHeight / vTotalheight);
    sum = x;

    // hack to make spritesheets scroll in the y dimension
    while (sum > 1.0f)
    {
        y += vHeight / vTotalheight;
        sum--;
    }

    if (vFlip > 0.5)
    {
        x = -x;
    }
    if (vFlipVertical > 0.5)
    {
        y = -y;
    }

    coords.x = x;
    coords.y = y;

    final = texture(textureSampler, coords);
/*#else
    int i = int(vTextureIndex);
    final = texture(Combined[nonuniformEXT(i)], coords, 0.0);
#endif*/

    if (final.a < 0.9f)
        discard;
    if (final.r > 0.9f && final.g < 0.1f && final.b > 0.9f)
        discard;

    //final += o_colourTint;

    vFragColor = final;
}
