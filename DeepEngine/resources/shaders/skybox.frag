#version 330 core
#pragma debug(on)

out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, texCoord);
}
