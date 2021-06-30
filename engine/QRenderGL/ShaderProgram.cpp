#include "Base_include.h"
#include "ShaderProgram.h"
#include "Shader.h"

#define GLEW_STATIC
#include <GL/glew.h>

ShaderProgram::ShaderProgram(const char* vert_path, const char* frag_path)
{
	Shader vert = Shader(vert_path, GL_VERTEX_SHADER);
	Shader frag = Shader(frag_path, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vert.id);
	glAttachShader(program, frag.id);
	glLinkProgram(program);

	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		GetLogManagerEx()->LogError("Shader program link error! Details:\n%s", true, infoLog);
	}
	
	//Autodelete shaders
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::use()
{
	glUseProgram(program);
}
