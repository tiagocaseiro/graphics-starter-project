#include "Shader.h"

#include <fstream>

bool Shader::loadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    GLuint vertexShader = readShader(vertexShaderFilename, GL_VERTEX_SHADER);
    if(!vertexShader)
    {
        return false;
    }
    GLuint fragmentShader = readShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);
    if(!fragmentShader)
    {
        return false;
    }

    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertexShader);
    glAttachShader(mShaderProgram, fragmentShader);
    glLinkProgram(mShaderProgram);

    GLint isProgramLinked;

    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &isProgramLinked);

    if(!isProgramLinked)
    {
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::use()
{
}

void Shader::cleanup()
{
    glDeleteProgram(mShaderProgram);
}

GLuint Shader::readShader(const std::string shaderFilename, GLuint shaderType)
{
    std::string shaderAsText;
    std::ifstream inFile(shaderFilename);

    if(inFile.is_open())
    {
        inFile.seekg(0, std::ios::end);
        shaderAsText.reserve(inFile.tellg());
        inFile.seekg(0, std::ios::beg);
        shaderAsText.assign((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();
    }
    else
    {
        return 0;
    }

    if(inFile.bad() || inFile.fail())
    {
        inFile.close();
        return 0;
    }

    inFile.close();

    const char* shaderSource = shaderAsText.c_str();

    GLuint shader = glCreateShader(shaderType);

    glShaderSource(shader, 1, (const GLchar**)&shaderSource, 0);
    glCompileShader(shader);

    GLint isShaderCompiled;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &isShaderCompiled);
    if(!isShaderCompiled)
    {
        return 0;
    }

    return shader;
}
