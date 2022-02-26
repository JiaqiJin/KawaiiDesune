#pragma once

#include "../Render/Application.h"
#include "../Render/Window.h"

class Demo
{
public:
	Application application;

	Demo(const char* appName = "DEMO APPLICATION", int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT);
	virtual ~Demo() = default;

	virtual void Update();
	void Run();
};