#version 330 core
out vec4 FragColor;

in vec3 color;
in float dist;
void main()
{
    FragColor = vec4(color/(1.f+dist), 1.);
}