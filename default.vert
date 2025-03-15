#version 330 core
layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 debug_color;

out float distance;
out vec3 color;

void main() {

    vec4 result = projection * view * model * aPos;
    gl_Position = result;
    if(result.z<0){
        color = aColor;
    }else{
        color = aColor * 0.3f;
    } 
}