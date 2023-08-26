#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertexColor;

uniform mat3 model;

void main()
{
	vec3 pos = model * vec3(aPos, 1.0f);

	gl_Position = vec4(pos, 1.0f);
	vertexColor = aColor / 255;
}
