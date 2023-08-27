#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;

void main()
{
	gl_Position = view * model * vec4(aPos, 0.0f, 1.0f);
	vertexColor = aColor / 255;
}
