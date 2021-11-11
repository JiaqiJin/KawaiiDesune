#pragma once

#include <string>
#include <memory>

#include "Config.h"
#include "../Interface/IRuntimeModule.h"

namespace Kawaii
{
    struct GfxConfiguration 
    {
        GfxConfiguration(std::string_view appName = APP_NAME, uint32_t width = WIDTH, uint32_t height = HEIGHT)
            : appName(appName),
            screenWidth(width),
            screenHeight(height)
        {

        }

        std::string      appName;
        uint32_t         msaaSample;
        uint32_t         screenWidth;
        uint32_t         screenHeight;
    };

	class Application : public IRuntimeModule
	{
    public:
        struct Rect
        {
            long left, top, right, bottom;
        };
        Application(GfxConfiguration& cfg);
        ~Application();
        int  Initialize() override;
        void Finalize() override;
        void Tick() override;

        virtual void* GetWindow() = 0;
        virtual bool  IsQuit();
        virtual void  SetCommandLineParameters(int argc, char** argv);
        virtual GfxConfiguration& GetConfiguration();
        virtual void UpdateInputEvent() = 0;

    protected:
        static bool      m_Quit;
        GfxConfiguration m_Config;
        int              m_ArgSize;
        char** m_Arg;
        Rect             m_Rect;

        double      m_FPS;
        short       m_FPSLimit = 60;
        uint64_t    m_FrameIndex = 1;
    private:
        Application() = default;
    };

	extern std::unique_ptr<Application> g_App;
}