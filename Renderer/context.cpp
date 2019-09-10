#include "context.h"

//SystemHeader
#include <iostream>

//LibraryHeader
#include "glew/GL/glew.h"
#include "glfw/glfw3.h"

bool Context::Init(int _width, int _height, const char * _title)
{
	glfwInit();

	Window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);

	glfwMakeContextCurrent(Window);

	glewInit();

	std::cout << "OpenGL Version: " << (const char *)glGetString(GL_VERSION) << "\n";
	std::cout << "Renderer: " << (const char *)glGetString(GL_RENDERER) << "\n";
	std::cout << "Vendor: " << (const char *)glGetString(GL_VENDOR) << "\n";
	std::cout << "GLSL: " << (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);//Color screen is set to when Clear Screen

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	
	return true;
}

void Context::Tick()
{
	glfwPollEvents();
	glfwSwapBuffers(Window);
}

void Context::Term()
{
	glfwDestroyWindow(Window);
	glfwTerminate();

	Window = nullptr;
}

bool Context::ShouldClose() const
{
	return glfwWindowShouldClose(Window);
}

void Context::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}