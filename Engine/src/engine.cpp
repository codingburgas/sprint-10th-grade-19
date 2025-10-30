#include "engine.h"

Engine::Engine()
	: window(inputStream)
{}

bool Engine::processMessage()
{
	return window.dispatchMessage();
}
