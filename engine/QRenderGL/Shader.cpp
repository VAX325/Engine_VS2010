#include "Base_include.h"
#include "Shader.h"

#define GLEW_STATIC
#include <GL/glew.h>

Shader::Shader(const char* path, int type)
{
    if (type == GL_VERTEX_SHADER)
    {
        _type = GL_VERTEX_SHADER;
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        _type = GL_FRAGMENT_SHADER;
    }
    else
    {
        GetLogManagerEx()->LogError("Invalid shader type!", true);
    }

    const char* source = GetFileSystemEx()->ReadFromFile(path);
    //source[strlen(source) + 1] = '\0';

    id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        GetLogManagerEx()->LogError("%i shader compiling error! Details:\n%s", true, type, infoLog);
    }
}

Shader::~Shader()
{
    glDeleteShader(id);
}

int Shader::GetType()
{
    return _type;
}
