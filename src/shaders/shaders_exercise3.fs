#version 330 core
out vec4 FragColor;
in vec3 postion;
void main()
{
    FragColor = vec4(postion, 1.0);
}