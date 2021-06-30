#pragma once
#ifndef RENDERGL_H
#define RENDERGL_H

class RenderManager;
enum class RenderableType;
struct GLFWwindow;

class RenderGL : public RenderManager
{
public:
	RenderGL();
	virtual ~RenderGL();

	typedef void(*PreRenderFunction)();
	typedef void(*RenderFunction)();
	typedef void(*PostRenderFunction)();
	
	virtual void SetPreRender(PreRenderFunction pre_function);
	virtual void SetRender(RenderFunction render_function);
	virtual void SetPostRender(PostRenderFunction post_function);

	virtual void Render();

	virtual IRenderable* CreateRenderable(RenderableType type, ...);

private:
	GLFWwindow* window;
	int width, height;
};

QRENDERGL_PUBLIC RenderManager* CreateGLRender();

#endif

