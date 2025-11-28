#include "window.h"

const LPCWSTR Window::WindowClass::name = L"OurGameWindow";
const HINSTANCE Window::WindowClass::executable = GetModuleHandle(nullptr);
Window::WindowClass Window::WindowClass::windowClass;

Window::Window()
	: windowStyle(WS_OVERLAPPEDWINDOW)
	, initialClientSize(1280, 720)
{
	RECT windowRect{0, 0, initialClientSize.width, initialClientSize.height};
	AdjustWindowRect(&windowRect, windowStyle, /*bMenu=*/ false);

	handle = CreateWindow(
		WindowClass::name, L"Sync", windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr, nullptr, WindowClass::executable, nullptr
	);

	ShowWindow(handle, SW_SHOW);
}

bool Window::dispatchMessages()
{
	MSG message{};

	while (PeekMessage(&message, handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	if (message.message == WM_NULL && !IsWindow(handle))
	{
		return false;
	}

	return true;
}

HWND Window::getHandle() const
{
	return handle;
}

void Window::overrideWindowProcess(WNDPROC customWindowProcess)
{
	SetWindowLongPtr(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(customWindowProcess));
}

Window::ClientSize Window::getClientSize() const
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	return {
		static_cast<UINT>(clientRect.right - clientRect.left),
		static_cast<UINT>(clientRect.bottom - clientRect.top)
	};
}

Window::ClientSize Window::getInitialClientSize() const
{
	return initialClientSize;
}

Window::WindowClass::WindowClass()
{
	WNDCLASS ourGameWindowClass{};
	ourGameWindowClass.style = CS_OWNDC;
	ourGameWindowClass.lpfnWndProc = DefWindowProc;
	ourGameWindowClass.hInstance = executable;
	ourGameWindowClass.lpszClassName = name;
	RegisterClass(&ourGameWindowClass);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(name, executable);
}
