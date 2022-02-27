#include "../PrecompiledHeader.h"
#include "Demo.h"

Demo::Demo(const char* appName, int width, int height) : 
	application(appName, width, height)
{
}

void Demo::Update()
{
	// Check for opengl errors
	application.PollOpenGlErrors();

	application.Update(1.0f);

	// Clear the window
	application.window.Clear();

	// PS: You want to have all logic and that stuff before this call 
	// Clear buffers and poll
	application.window.UpdateBuffers();
}

void Demo::Run()
{
	Update();
}