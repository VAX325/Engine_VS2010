#pragma once
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

class IRenderableGL;

class ShaderProgram
{
public:
	friend IRenderableGL;

	ShaderProgram(const char* vert_path, const char* frag_path);
	~ShaderProgram();

	void use();

private:
	unsigned int program;
};

#endif