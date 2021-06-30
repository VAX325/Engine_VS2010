#pragma once
#ifndef SHADER_H
#define SHADER_H

class ShaderProgram;

class Shader
{
public:
	friend ShaderProgram;

	Shader(const char* path, int type);
	~Shader();

	int GetType();
private:
	int _type;
	unsigned int id;
};

#endif