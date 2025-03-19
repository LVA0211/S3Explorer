#version 330 core
#define PI 3.1415926535897932384626433832795

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool is_back_hemisphere;

out float light;
out vec2 TexCoord;

const vec4 localCam = vec4(0,0,0,1);

void main() {
    
    vec4 local = normalize(view * model * aPos);
    vec4 localNormal = normalize(view * model * aNormal);

    vec4 projected = projection * local;
    gl_Position = projected;

    float cosdist = dot(localCam,local);

    float dist = acos(cosdist);

    if(is_back_hemisphere){
        dist = 2.f*PI - dist;
    }

    light = abs(dot(normalize(localCam-cosdist * local), localNormal))/ ( 1.f + dist);
    TexCoord = aUV;
}