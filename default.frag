#version 330 core

out vec4 FragColor;

uniform sampler2D diffuseTex;
uniform bool uses_texture;
uniform vec3 diffuse_color;

uniform vec4 scale;

in vec2 TexCoord;

noperspective in float light;

const vec4 viewCam = vec4(0.f,0.f,0.f,1.f);

void main()
{
    if(uses_texture){
        FragColor = vec4(texture(diffuseTex,TexCoord).rgb * light, 1.f);
    } else {
        FragColor = vec4(diffuse_color*light,1.f);
    }
}