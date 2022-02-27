#include "../PrecompiledHeader.h"
#include "Window.h"

Window::Window(const char* windowName, int width, int height, int fps, bool vsync) : width(width), height(height), fps(fps), vsync(vsync)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW HAS NOT INITIALIZED PROPERLY" << std::endl;
		glfwTerminate();
		assert(false);
	}

	// Create a window and make it the current context
	window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (!window)
	{
		std::cout << "GLFW WINDOW HAS NOT INITIALIZED PROPERLY" << std::endl;
		glfwTerminate();
		assert(false);
	}

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "COULD NOT INITIALIZE GLAD" << std::endl;
		glfwTerminate();
		assert(false);
	}

	// Vsync 
	glfwSwapInterval(vsync);
}

Window::~Window()
{

	// Terminate window
	glfwDestroyWindow(window);
}