#include "Base_include.h"
#include "../QRender/QRender.h"
#include "RenderGL.h"
#include "IRenderableGL.h"
#include <stb/stb_image.h>
#include "ShaderProgram.h"

//OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

void error_callback(int error, const char* msg) {
	std::string s;
	s = " [" + std::to_string(error) + "] " + msg + '\n';
	GetLogManagerEx()->LogError(s.c_str(), false);
}

void resize_callback(GLFWwindow* wnd, int width, int height)
{
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
}

RenderGL::RenderGL()
{
	int init = glfwInit();
	if(init != GL_TRUE)
	{
		GetLogManagerEx()->LogError("Can't init the GLFW!", true);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(800, 640, "QEngine", nullptr, nullptr);
	if (window == nullptr)
	{
		GetLogManagerEx()->LogError("Failed to create GLFW window", true);
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		GetLogManagerEx()->LogError("Failed to initialize GLEW", true);
	}

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0.0f, 1024, 768, 0.0f, -1, 1);
	
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glfwSetErrorCallback(error_callback);
	glfwSetWindowSizeCallback(window, resize_callback);

	glEnable(GL_DEPTH_TEST);

	GetLogManagerEx()->LogMsg("OpenGL Init - ok");
}

RenderGL::~RenderGL()
{
	//glfwTerminate();
}

void RenderGL::SetPreRender(PreRenderFunction pre_function)
{
	preFunction = pre_function;
}

//That function deprecated! Don't use it
void RenderGL::SetRender(RenderFunction render_function)
{
	//renderFunction = render_function;
}

void RenderGL::SetPostRender(PostRenderFunction post_function)
{
	postFunction = post_function;
}

void RenderGL::SetShutdown(ShutdownFunction shutdown_function)
{
	shutdownFunction = shutdown_function;
}

void RenderGL::Render()
{
	while (!glfwWindowShouldClose(window))
	{
		if (preFunction)
			preFunction();

		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(int i = 0; i != render_objects.size(); i++)
		{
			render_objects[i]->Render();
		}

		glfwSwapBuffers(window);

		if (postFunction)
			postFunction();
	}

	if(shutdownFunction)
		shutdownFunction();

	glfwTerminate();
}

#include <cstdarg>

const char* default_frag_source;
const char* defualt_vert_source;

IRenderable* RenderGL::CreateRenderable(RenderableType type, ...)
{
	switch (type)
	{
	case RenderableType::NONE:
	{
		return nullptr;
	}
		break;
	case RenderableType::SPRITE:
	{
		Texture* texture = new Texture();
		IRenderableGL* sprite = new IRenderableGL();
		sprite->_type = RenderableType::SPRITE;

		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		va_list args;
		va_start(args, type);

		const char* path = va_arg(args, const char*);
		int x = va_arg(args, int);
		int y = va_arg(args, int);
		int w = va_arg(args, int);
		int h = va_arg(args, int);
		const char* vert = va_arg(args, const char*);
		const char* frag = va_arg(args, const char*);
		unsigned int layer = va_arg(args, unsigned int);

		bool default_vert = false;
		bool default_frag = false;

		if(!vert)
		{
			default_vert = true;
		}

		if(!frag)
		{
			default_frag = true;
		}

		ShaderProgram* shaderProgram = new ShaderProgram(default_vert ? "../gamedata/shaders_gl/default_vert.vs" : vert, default_frag ? "../gamedata/shaders_gl/default_frag.fs" : frag);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

		if (!data)
		{
			GetLogManagerEx()->LogError("Can't load the \"%s\"!", false, path);
			return nullptr;
		}

		sprite->x = x;
		sprite->y = y;
		sprite->width = w != -1 ? w : width;
		sprite->height = h != -1 ? h : height;
		sprite->layer = layer;

		if(w == -2)
		{
			sprite->width = this->width;
		}
		if(h == -2)
		{
			sprite->height = this->height;
		}

		va_end(args);

		sprite->Rebuild();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		sprite->texture = texture;
		sprite->shaderProgram = shaderProgram;

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);

		render_objects.push_back(sprite);

		return sprite;
	}
		break;
	default:
	{
		return nullptr;
	}
		break;
	}
}

RenderGL* render;

QRENDERGL_PUBLIC RenderManager* CreateGLRender()
{
	render = new RenderGL();

	return dynamic_cast<RenderManager*>(render);
}
