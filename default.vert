#version 330 core
#define PI 3.1415926535897932384626433832795

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float dist;
out vec3 color;

void main() {
    
    vec4 local = view * model * aPos;
    vec4 projected = projection * local;
    gl_Position = projected;

    dist = acos(local.w/length(local));

    if(projected.z<0){
        color = vec3(1.0f,0.f,0.f);
    } else {
        dist = 2.f * PI - dist;
        color = vec3(0.0f,1.f,0.f);
    }
}