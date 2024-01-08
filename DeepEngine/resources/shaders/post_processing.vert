#version 330 core
#pragma debug(on)

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 texCoord;

void main()
{
    texCoord = aTexCoord;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}