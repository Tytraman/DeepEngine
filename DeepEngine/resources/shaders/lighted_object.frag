#version 330 core
#pragma debug(on)

out vec4 FragColor;

uniform vec3 deObjectColor;
uniform vec3 deLightColor;

void main()
{
    FragColor = vec4(deLightColor * deObjectColor, 1.0);
}
