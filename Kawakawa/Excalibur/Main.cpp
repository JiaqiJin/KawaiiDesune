#include <iostream>
#include "Platform/Window/WindowsApplication.h"

using namespace Excalibur;

namespace Excalibur
{
	extern Application* GApp = new WindowsApplication();
}

int main()
{
	GApp->Initialize();

	GApp->mWorld->LoadScene("Asset/Model/aili_cycle.fbx");
	GApp->mWorld->DumpEntities();

	GApp->Run();
	GApp->Finalize();
	return 0;
}