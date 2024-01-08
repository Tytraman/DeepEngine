#version 330 core
#pragma debug(on)

out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D myTex;

void main()
{
    FragColor = texture(myTex, texCoord) * vertexColor;
}
