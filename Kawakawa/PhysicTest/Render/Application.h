#pragma once

#include "Window.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

class Application
{
public:
	Window window;
	float cameraSensitivity;

	Application(const char* appName = "Kawaii", 
		int width = DEFAULT_WINDOW_WIDTH, 
		int height = DEFAULT_WINDOW_HEIGHT, 
		int fps = DEFAULT_FPS);

	~Application() = default;

	void Update(float deltaTime);

	inline void SetCameraSensitivity(float sensitivity) { cameraSensitivity = sensitivity; }

	inline void PollOpenGlErrors()
	{
		// Poll for opengl errors
		while (GLenum error = glGetError() != GL_NO_ERROR)
		{
			std::cout << "OpenGL error : " <<  error << std::endl;
		}
	}
};