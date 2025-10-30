#pragma once
#include "window.h"

class Engine
{
public:
	Engine();

	bool processMessage();

private:
	Window window;
	Window::InputStream inputStream;
};
