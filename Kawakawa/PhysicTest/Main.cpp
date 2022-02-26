#include <iostream>

#include "Demos/Demo.h"

int main()
{
	Demo* app = new Demo();
	// Run application
	while (true)
	{
		app->Run();
	}

	return 0;
}