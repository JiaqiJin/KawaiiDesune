#include <iostream>
#include "Core/Application/WindowsApplication.h"

using namespace std;
using namespace Excalibur;

namespace Excalibur {
	extern Application* GApp = GWindowsApplication::GetInstance();
}




int main()
{
	CHECK_APPLICATION_INIT(GApp->Initialize());
	GApp->m_World->LoadScene("Asset/Model/Drum/NAMDrum01.obj");
	GApp->m_World->DumpEntities();

	GApp->Run();
	GApp->Finalize();

}