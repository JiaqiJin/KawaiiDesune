#pragma once

#include "GameTimer.h"
#include "RHI/DX12GraphicRHI.h"

class Engine
{
public:
    Engine(HINSTANCE HInstance);
    Engine(const Engine& rhs) = delete;
    Engine& operator=(const Engine& rhs) = delete;
    virtual ~Engine();

public:
    static Engine* GetEngineSingleton();

    HINSTANCE GetEngineInstHandle()const;
    HWND      GetMainWnd()const;

    bool Initialize();
    int Run();
    bool Destroy();

    LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    int GetWindowWidth() { return WindowWidth; }

    int GetWindowHeight() { return WindowHeight; }

private:
    bool InitMainWindow();
    void OnResize();
    void CalculateFrameStats();

protected:

    void Update(const GameTimer& gt);
    void EndFrame(const GameTimer& gt);

    void OnMouseDown(WPARAM btnState, int x, int y) { }
    void OnMouseUp(WPARAM btnState, int x, int y) { }
    void OnMouseMove(WPARAM btnState, int x, int y) { }
    void OnMouseWheel(float WheelDistance) { }
protected:

    static Engine* EngineSingleton;
    std::wstring WindowTile = L"TotoroEngine";

    bool bInitialize = false;

    HINSTANCE EngineInstHandle = nullptr; // application instance handle
    HWND      MainWindowHandle = nullptr; // main window handle
    bool      bAppPaused = false;  // is the application paused?
    bool      bAppMinimized = false;  // is the application minimized?
    bool      bAppMaximized = false;  // is the application maximized?
    bool      bResizing = false;   // are the resize bars being dragged?
    bool      bFullscreenState = false;// fullscreen enabled

    int WindowWidth = 1280;
    int WindowHeight = 720;

    GameTimer Timer;

    std::unique_ptr<RHI::DX12GraphicRHI> D3D12RHI;
};