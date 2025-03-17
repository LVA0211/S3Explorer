#version 330 core
#define PI 3.1415926535897932384626433832795

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float light;
out vec3 color;

const vec4 localCam = vec4(0,0,0,1);

void main() {
    
    vec4 local = normalize(view * model * aPos);
    vec4 localNormal = normalize(view * model * aNormal);

    vec4 projected = projection * local;
    gl_Position = projected;

    light = dot(normalize(localCam-dot(localCam,local) * local), localNormal);

    if(projected.z<0){
        color = vec3(1.f,0.f,0.f);
    } else {
        color = vec3(0.0f,1.f,0.f);
    }
}