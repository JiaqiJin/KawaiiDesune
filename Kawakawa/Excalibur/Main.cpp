#include <iostream>
#include "Platform/Window/WindowsApplication.h"

using namespace Excalibur;

extern Application* GApp = GWindowsApplication::GetInstance();

int main()
{
	GApp->Initialize();
	GApp->Run();
	GApp->Finalize();
	return 0;
}