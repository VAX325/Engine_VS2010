#include "Base_include.h"
#include "../QRender/QRender.h"
#include "IRenderableGL.h"
#include "ShaderProgram.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float vertices[] = {
	// координаты        // цвета            // текстурные координаты
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // верхн€€ права€
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // нижн€€ права€
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // нижн€€ лева€
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // верхн€€ лева€ 
};

unsigned int indices[] = {
		0, 1, 3, // первый треугольник
		1, 2, 3  // второй треугольник
};

//“ут до пизды стара€ функци€ которую € адаптировал под glm (€ ленивый и не нашел более новую)
inline glm::highp_vec3 DisplayToOrtho(int x, int y)
{
	int vp[4];
	double mMatrix[16], pMatrix[16];
	double ox;
	double oy;
	double oz;
	float z;

	glGetIntegerv(GL_VIEWPORT, vp);
	y = vp[3] - y - 1;

	glGetDoublev(GL_MODELVIEW_MATRIX, mMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, pMatrix);
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	gluUnProject(x, y, z, mMatrix, pMatrix, vp, &ox, &oy, &oz);

	float ox_f = (float)ox;
	float oy_f = (float)oy;
	float oz_f = /*(float)oz*/0.0f;

	return glm::vec3(ox_f, oy_f, oz_f);
}

inline glm::highp_vec3 SizeToOrtho(int width, int height)
{
	glm::vec3 vect = glm::vec3(0.1f, 0.1f, 0.0f);
	return vect;
}

IRenderableGL::IRenderableGL()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//  оординатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ÷ветовые атрибуты
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// јтрибуты текстурных координат
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	texture = { 0 };

	shaderProgram = nullptr;
}

IRenderableGL::~IRenderableGL()
{
}

void IRenderableGL::Render()
{
	switch (_type)
	{
	case RenderableType::NONE:
		//No render
		break;
	case RenderableType::SPRITE:
	{
		//Use texture
		glBindTexture(GL_TEXTURE_2D, texture->id);

		glUniform1i(glGetUniformLocation(shaderProgram->program, "Texture"), 0);

		shaderProgram->use();

		int w = int(width / 2);
		int h = int(height / 2);

		glm::vec3 ortho_cords = DisplayToOrtho(x + w, y + h);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, ortho_cords);
		trans = glm::scale(trans, SizeToOrtho(width, height));

		GLuint transformLoc = glGetUniformLocation(shaderProgram->program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
		break;
	default:
		GetLogManagerEx()->LogError("IRenderable 0x%08x has no type!", false, this);
		break;
	}
}

