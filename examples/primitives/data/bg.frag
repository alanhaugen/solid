#version 330 core
//es

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

#ifdef VULKAN
//input form the vertex shader
layout(location = 0) out vec4 vFragColor;	//fragment shader output
layout(binding=0) uniform sampler2D textureSampler;

layout(location = 0) in vec4 vSmoothColor;
layout(location = 1) in vec2 vSmoothTexcoord;
layout(location = 2) in float vTime;
#else

layout(location=0) out vec4 vFragColor;	//fragment shader output

//input form the vertex shader
smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader
smooth in vec2 vSmoothTexcoord;
uniform sampler2D textureSampler;
uniform bool uEnableTexture;

in float vTime;
#endif

vec2 rotate2D(vec2 _st, float _angle){
    _st -= 0.5;
    _st =  mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle)) * _st;
    _st += 0.5;
    return _st;
}

vec2 tile(vec2 _st, float _zoom){
    _st *= _zoom;
    return fract(_st);
}

#define PI 3.14159265358979323846


float box(vec2 _st, vec2 _size, float _smoothEdges){
    _size = vec2(0.5)-_size*0.5;
    vec2 aa = vec2(_smoothEdges*0.5);
    vec2 uv = smoothstep(_size,_size+aa,_st);
    uv *= smoothstep(_size,_size+aa,vec2(1.0)-_st);
    return uv.x*uv.y;
}

void main()
{
    vec2 st = gl_FragCoord.xy/vec2(1024,1024).xy;
    vec3 color = vSmoothColor.xyz;

    // Divide the space in 4
    st = tile(st,10.);

    // Use a matrix to rotate the space 45 degrees
    st = rotate2D(st,PI*0.25f);

    // Draw a square
    vec3 newColor = vec3(box(st,vec2(sin(vTime / 4.0f) + 0.75),0.01));

    if (newColor != vec3(0.0))
    {
        color = vec3(255.0f/255.f, 199.0f/255.f, 143.0f/255.f);
    }

    // color = vec3(st,0.0);

    vFragColor = vec4(color,1.0);
}

