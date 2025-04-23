#pragma once

#include <string>

#include <glad/glad.h>

class Shader
{
public:
    bool loadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
    void use();
    void cleanup();

private:
    GLuint mShaderProgram = 0;
    GLuint readShader(const std::string shaderFilename, GLuint shaderType);
};
