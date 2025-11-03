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
	struct ClientSize
	{
		UINT width;
		UINT height;
	};

	class InputStream;

	Window(InputStream& outInputStream);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool dispatchMessage() const;

	HWND getHandle() const;

	ClientSize getClientSize() const;

private:
	static LRESULT CALLBACK setupWindowProcess(HWND, UINT message, WPARAM, LPARAM);
	static LRESULT CALLBACK mainWindowProcess(HWND, UINT message, WPARAM, LPARAM);

	HWND handle;

	DWORD windowStyle;
};

// Facilitates access to a window's mouse, keyboard, resize input.
// Passed to the window's constructor.
class Window::InputStream
{
	friend LRESULT CALLBACK Window::setupWindowProcess(HWND, UINT message, WPARAM, LPARAM);
	friend LRESULT CALLBACK Window::mainWindowProcess(HWND, UINT message, WPARAM, LPARAM);

	LRESULT processMessage(HWND, UINT message, WPARAM, LPARAM);
};
