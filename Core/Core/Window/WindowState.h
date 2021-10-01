#pragma once

namespace Kawaii
{
    struct WindowState
    {
        enum class WindowMode
        {
            Windowed,
            Borderless,
            Fullscreen
        };

        int x;
        int y;
        int width;
        int height;
        int presentInterval; 
        WindowMode windowMode;

        static WindowState GetDefault();
    };
}