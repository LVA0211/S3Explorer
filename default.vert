#version 330 core
#define PI 3.1415926535897932384626433832795

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 model;
uniform vec4 scale;
uniform mat4 view;
uniform mat4 projection;
uniform bool is_back_hemisphere;

out float light;
out vec2 TexCoord;


const vec4 localCam = vec4(0,0,0,1);

void main() {

    mat4 scalematrix = mat4(scale.x,0.f,0.f,0.f, 0.f,scale.y,0.f,0.f, 0.f,0.f,scale.z,0.f, 0.f,0.f,0.f,scale.w);
    
    vec4 local = normalize(view * model * scalematrix * aPos);

    mat4 normalMatrix = transpose(inverse(view * model * scalematrix));

    vec4 localNormal = normalMatrix * aNormal;

    localNormal = normalize(localNormal - dot(localNormal,local) * local);

    vec4 projected = projection * local;
    gl_Position = projected;

    float cosdist = dot(localCam,local);

    float dist = acos(cosdist);

    if(is_back_hemisphere){
        dist = 2.f*PI - dist;
    }

    light = abs(dot(normalize(localCam-cosdist * local), localNormal))/( 1.f + dist*0.5f);
    TexCoord = aUV;
}