#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

out vec4 texColor;
out vec2 texCoord;

void main()
{
    vec3 offset = vec3(0.0, 0.0, -1.0);
    gl_Position = projection * view * vec4(aPos + offset, 1.0);
    texCoord    = aTexCoord;
    texColor    = vec4(aColor, 1.0);
}