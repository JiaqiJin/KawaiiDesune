#include "GraphicsMgrGL.h"

#include <iostream>
#include <GL/glew.h>

namespace Excalibur
{
	int GraphicsMgrGL::Initialize() noexcept
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			printf("Error: %s\n", glewGetErrorString(err));
		}

		// Enable depth testing.
		glClearDepth(1.0f);
		// Enable depth testing.
		glEnable(GL_DEPTH_TEST);

		return 0;
	}

	void GraphicsMgrGL::Finalize() noexcept
	{

	}

	void GraphicsMgrGL::Tick() noexcept
	{
	}

}