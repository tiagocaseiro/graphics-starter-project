#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>

#include "tools/Macros.h"

SHARED_ONLY(Shader)

class Shader
{
public:
    static ShaderShared make(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
    void use();
    ~Shader();

private:
    Shader(GLuint mShaderProgram);

    const GLuint mShaderProgram = 0;
};
