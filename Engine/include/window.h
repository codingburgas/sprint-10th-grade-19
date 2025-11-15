#include <Windows.h>

class Window
{
public:
	struct ClientSize
	{
		UINT width;
		UINT height;
	};

	Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool dispatchMessage();

	HWND getHandle() const;

	ClientSize getClientSize() const;
	ClientSize getInitialClientSize() const;

	void overrideWindowProcess(WNDPROC customWindowProcess);

private:
	HWND handle;

	DWORD windowStyle;

	const ClientSize initialClientSize;

	class WindowClass
	{
	public:
		static const HINSTANCE executable;
		static const LPCWSTR name;

	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static WindowClass windowClass;
	};
};
