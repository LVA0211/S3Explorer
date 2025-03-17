#version 330 core
out vec4 FragColor;

in vec3 color;
in float light;
void main()
{
    FragColor = vec4(color*light, 1.);
}