#include <Windows.h>
#include "engine.h"
#include <GeometricPrimitive.h>
#include "camera.h"
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	Engine& engine = Engine::getInstance();

	DirectX::GeometricPrimitive::VertexCollection wallVertices;
	DirectX::GeometricPrimitive::IndexCollection wallIndices;
	DirectX::GeometricPrimitive::CreateBox(wallVertices, wallIndices, {4, 3, 0});
	auto wall = Engine::getInstance().makeGeometricPrimitive(wallVertices, wallIndices);

	Camera& camera = engine.getCamera();

	while (engine.processMessage())
	{
		engine.renderFrame([&](ID3D11DeviceContext* dc)
		{
			wall->Draw(DirectX::SimpleMath::Matrix::CreateWorld({0, 0, -1}, {0, 0, -1}, {0, 1, 0}), camera.getViewMatrix(), camera.getProjectionMatrix());
		});
	}
}
