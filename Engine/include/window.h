#include <Windows.h>

class Window
{
private:
	class WindowClass
	{
	public:
		static LPCWSTR getClassName();
		static HINSTANCE getInstance();

	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static LPCWSTR className;
		static WindowClass windowClass;
		HINSTANCE hInstance;
	};

public:
	class InputStream;

	Window(InputStream& outInputStream);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool dispatchMessage() const;

private:
	static LRESULT CALLBACK setupWindowProcess(HWND, UINT message, WPARAM, LPARAM);
	static LRESULT CALLBACK mainWindowProcess(HWND, UINT message, WPARAM, LPARAM);

	HWND windowHandle;

	DWORD windowStyle;
};

class Window::InputStream
{
	friend LRESULT CALLBACK Window::setupWindowProcess(HWND, UINT message, WPARAM, LPARAM);
	friend LRESULT CALLBACK Window::mainWindowProcess(HWND, UINT message, WPARAM, LPARAM);

	LRESULT processMessage(HWND, UINT message, WPARAM, LPARAM);
};
