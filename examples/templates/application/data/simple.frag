#version 330 core

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

#ifdef VULKAN
//#extension GL_EXT_nonuniform_qualifier : require
//input form the vertex shader
layout(location = 0) out vec4 vFragColor;	//fragment shader output

layout(set=1, binding=0) uniform sampler2D textureSampler;
layout(set=1, binding=0) uniform sampler2D Combined[];

layout(location = 0) in vec4 vSmoothColor;
layout(location = 1) in vec2 vSmoothTexcoord;
layout(location = 2) in float vTextureIndex;
#else
out vec4 vFragColor;	//fragment shader output

smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader
smooth in vec2 vSmoothTexcoord;
uniform sampler2D textureSampler;
uniform bool uEnableTexture;
#endif

void main()
{
    vec4 final = vSmoothColor;

#ifdef VULKAN
    bool uEnableTexture = false;

    if (vTextureIndex > -0.5f)
    {
        uEnableTexture = true;
    }
#endif

    if (uEnableTexture == true)
    {
        final.x = vSmoothTexcoord.x;
        final.y = vSmoothTexcoord.y;
        final = texture(textureSampler, vSmoothTexcoord);
    }

    vFragColor = final;
}

