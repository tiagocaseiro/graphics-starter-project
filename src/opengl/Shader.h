#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>

class Shader
{
public:
    static std::shared_ptr<Shader> make(const std::string& vertexShaderFilename,
                                        const std::string& fragmentShaderFilename);
    void use();
    ~Shader();

private:
    Shader(GLuint mShaderProgram);

    const GLuint mShaderProgram = 0;
};
