#version 460 core

in vec4 texColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D Tex;

void main()
{
    FragColor = texture(Tex, texCoord) * texColor;
}