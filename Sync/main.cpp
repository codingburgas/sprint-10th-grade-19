#include <Windows.h>
#include "engine.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	Engine engine;

	while (engine.processMessage())
	{
		engine.renderFrame();
	}
}
