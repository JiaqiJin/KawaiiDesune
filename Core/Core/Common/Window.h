#pragma once

struct WindowMsg_t
{
	void* handle = nullptr;
	void* instance = nullptr;
	uint32_t msg = 0;
	uint64_t wparam = 0;
	int64_t lparam = 0;
	float width = 0;
	float height = 0;
};

struct WindowInitParam
{
	HINSTANCE instance;
	std::string title;
	float width, height;
	bool maximize;
};

class Window
{
public:
	static void Initialize(WindowInitParam const& init);

	static float GetWidth();
	static float GetHeight();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static void SetCallback(std::function<void(WindowMsg_t const& window_data)> callback);

	bool Loop();
	void Destroy();
	void* Handle();
	bool IsActive();

private:
	inline static HINSTANCE m_instance = nullptr;
	inline static HWND m_handle = nullptr;
	inline static std::function<void(WindowMsg_t const& window_data)> _msg_callback = nullptr;
};