#pragma once
#ifndef IRENDERABLEGL_H
#define IRENDERABLEGL_H

enum class RenderableType;

class IRenderable;
class RenderGL;
class ShaderProgram;

typedef unsigned int GLuint;

struct Texture
{
	GLuint id;
};

class IRenderableGL : public IRenderable
{
public:
	friend RenderGL;

	IRenderableGL();
	virtual ~IRenderableGL();

	virtual void Render();
	RenderableType GetType() { return _type; };

	virtual void SetLayer(unsigned int);

private:
	void Rebuild();

	Texture* texture;
	ShaderProgram* shaderProgram;
	GLuint VBO, VAO, EBO;
};

#endif