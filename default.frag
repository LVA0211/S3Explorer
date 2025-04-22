#version 330 core
#define PI 3.1415926535897932384626433832795f

out vec4 FragColor;

uniform sampler2D diffuseTex;
uniform bool uses_texture;
uniform vec3 diffuse_color;

uniform float normal_falloff;
uniform float light_falloff;
uniform bool is_back_hemisphere;

in float light;
in vec2 TexCoord;

in vec4 localNormal;
in vec4 local;

const vec4 localCam = vec4(0.f,0.f,0.f,1.f);

void main()
{
    float cosdist = dot(localCam,local);

    float dist = acos(cosdist);

    if(is_back_hemisphere){
        dist = 2.f*PI - dist;
    }

    float light = (1.f + normal_falloff * (abs(dot(normalize(localCam-cosdist * local), localNormal))-1.f))/(1.f + dist*light_falloff + dist*dist*light_falloff);

    if(uses_texture){
        FragColor = vec4(texture(diffuseTex,TexCoord).rgb * light, 1.f);
    } else {
        FragColor = vec4(diffuse_color,1.f);
    }
}