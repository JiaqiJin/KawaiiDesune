#include "GraphicsMgrGL.h"

#include <iostream>
#include <glad/glad.h>

namespace Excalibur
{
	int GraphicsMgrGL::Initialize() noexcept
	{
		int result;
		result = gladLoadGL();
		if (!result)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			return -1;
		}

		// Enable depth testing.
		glClearDepth(1.0f);
		// Enable depth testing.
		glEnable(GL_DEPTH_TEST);

		std::cout << "OpenGl Version: " << GLVersion.major << "." << GLVersion.minor << " loaded" << std::endl;

		return 0;
	}

	void GraphicsMgrGL::Finalize() noexcept
	{

	}

	void GraphicsMgrGL::Tick() noexcept
	{
	}

}