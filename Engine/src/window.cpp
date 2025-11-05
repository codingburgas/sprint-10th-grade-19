#include "window.h"

LPCWSTR Window::WindowClass::className = L"OurGameWindow";
Window::WindowClass Window::WindowClass::windowClass;

LRESULT Window::InputStream::processMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(windowHandle, message, wParam, lParam);
}

Window::Window(InputStream& outInputStream)
	: windowStyle(WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU)
{
	RECT windowRect{0, 0, 1280, 720};
	AdjustWindowRect(&windowRect, windowStyle, /*bMenu=*/ false);

	handle = CreateWindow(
		WindowClass::getClassName(), L"Game Title Caption Text", windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right, windowRect.bottom,
		nullptr, nullptr, WindowClass::getInstance(), &outInputStream
	);

	ShowWindow(handle, SW_SHOW);
}

Window::~Window()
{
	DestroyWindow(handle);
}

bool Window::dispatchMessage() const
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

Window::ClientSize Window::getClientSize() const
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	return {
		static_cast<UINT>(clientRect.right) - clientRect.left,
		static_cast<UINT>(clientRect.bottom) - clientRect.top
	};
}

LRESULT CALLBACK Window::setupWindowProcess(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE)
	{
		CREATESTRUCT* windowCreationStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		InputStream* outInputStream = reinterpret_cast<InputStream*>(windowCreationStruct->lpCreateParams);
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(outInputStream));
		SetWindowLongPtr(windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(mainWindowProcess));
		return outInputStream->processMessage(windowHandle, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(windowHandle, message, wParam, lParam);
	}
}

LRESULT CALLBACK Window::mainWindowProcess(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE)
	{
		DestroyWindow(windowHandle);
		return 0;
	}
	else
	{
		InputStream* outInputStream = reinterpret_cast<InputStream*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
		return outInputStream->processMessage(windowHandle, message, wParam, lParam);
	}
}

Window::WindowClass::WindowClass()
	: hInstance(GetModuleHandle(nullptr))
{
	WNDCLASS ourGameWindowClass{};
	ourGameWindowClass.style = CS_OWNDC;
	ourGameWindowClass.lpfnWndProc = setupWindowProcess;
	ourGameWindowClass.hInstance = hInstance;
	ourGameWindowClass.lpszClassName = className;
	RegisterClass(&ourGameWindowClass);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(className, hInstance);
}

LPCWSTR Window::WindowClass::getClassName()
{
	return className;
}

HINSTANCE Window::WindowClass::getInstance()
{
	return windowClass.hInstance;
}
