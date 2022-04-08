#pragma once

#include "Common/GameTimer.h"

class WinApplication
{
public:

	WinApplication(HINSTANCE hInstance);
	WinApplication(const WinApplication& rhs) = delete;
	WinApplication& operator=(const WinApplication& rhs) = delete;
	virtual ~WinApplication();

	static WinApplication* GetApp();

	HINSTANCE AppInst() const;
	HWND      MainWnd() const;

	bool Get4xMsaaState()const;
	void Set4xMsaaState(bool value);

	int Run();

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static WinApplication* mApp;

	bool Initialize();
	virtual void Update(const GameTimer& gt);

	bool InitMainWindow();

	void CalculateFrameStats();

	HINSTANCE mhAppInst = nullptr; // application instance handle
	HWND      mhMainWnd = nullptr; // main window handle
	bool      mAppPaused = false;  // is the application paused?
	bool      mMinimized = false;  // is the application minimized?
	bool      mMaximized = false;  // is the application maximized?
	bool      mResizing = false;   // are the resize bars being dragged?
	bool      mFullscreenState = false;// fullscreen enabled
	// Set true to use 4X MSAA (?.1.8).  The default is false.
	bool      m4xMsaaState = false;    // 4X MSAA enabled
	UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	int GetClientWidth() { return mClientWidth; }
	int GetClientHeight() { return mClientHeight; }
private:

	GameTimer mTimer;
	WNDCLASSEX wc;
	// Derived class should set these in derived constructor to customize starting values.
	std::wstring mMainWndCaption = L"DirectX12Lab";
	int mClientWidth = 1920;
	int mClientHeight = 1080;

	// Convenience overrides for handling mouse input.
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
	void OnKeyboardInput(const GameTimer& gt);

	POINT mLastMousePos;
};