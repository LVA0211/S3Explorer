#version 330 core
#define PI 3.1415926535897932384626433832795
#define DELTA 0.0000001f

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 model;
uniform vec4 scale;
uniform mat4 view;
uniform mat4 projection;
uniform bool is_back_hemisphere;

uniform float light_falloff = 0.1f;
uniform float normal_falloff = 0.f;

out float light;
out vec2 TexCoord;

out vec4 local;
out vec4 localNormal;

const vec4 localCam = vec4(0,0,0,1);

void main() {   
    

    local = normalize(view * model * (scale * aPos));

    localNormal = normalize(view * model * aNormal);


    gl_Position = projection * local;

    TexCoord = aUV;
}