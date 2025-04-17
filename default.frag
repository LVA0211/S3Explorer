#version 330 core
out vec4 FragColor;

uniform sampler2D diffuseTex;
uniform bool uses_texture;
uniform vec3 diffuse_color;

in float light;
in vec2 TexCoord;

in vec4 normal;

void main()
{
    if(uses_texture){
        FragColor = vec4(texture(diffuseTex,TexCoord).rgb * light, 1.f);
    } else {
        FragColor = vec4(diffuse_color,1.f);
    }
}