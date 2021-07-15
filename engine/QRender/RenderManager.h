#pragma once
#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

class IRenderable;
enum class RenderableType;

enum class RenderEngines
{
	SYSTEM_SELECTION,
#if IS_WIN32
	DIRECTX9,
	DIRECTX11,
#endif
#if IS_WIN10
	DIRECTX12,
#endif
	OPENGL,
	//In faaar future
	OPENGL_LEGACY, VULKAN,
};

#include <vector>

class RenderManager
{
public:
	RenderManager() {};
	virtual ~RenderManager() {};

	typedef void(__cdecl* PreRenderFunction)();
	typedef void(__cdecl* RenderFunction)();
	typedef void(__cdecl* PostRenderFunction)();
	typedef void(__cdecl* ShutdownFunction)();

	virtual void SetPreRender(PreRenderFunction pre_function) {};
	virtual void SetRender(RenderFunction post_function) {};
	virtual void SetPostRender(PostRenderFunction post_function) {};
	virtual void SetShutdown(ShutdownFunction shutdown_function) {};

	virtual void Render() {};

	/*
	1) SPRITE: CreateRenderable(RenderableType::SPRITE, const char* path, int x, int y, int w(optional), int h(optional) const char* shader_vert(optional), const char* shader_frag(optional))
	2) LINE: 
	*/
	virtual IRenderable* CreateRenderable(RenderableType type, ...) { return nullptr; };

protected:
	PreRenderFunction preFunction;
	RenderFunction renderFunction;
	PostRenderFunction postFunction;
	ShutdownFunction shutdownFunction;

	std::vector<IRenderable*> render_objects;
};

#endif