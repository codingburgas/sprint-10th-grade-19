#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <functional>
#include <GeometricPrimitive.h>
#include <wrl/client.h>
#include "camera.h"
#include "window.h"

class Engine
{
public:
	static Engine& getInstance();

	// Processes window input. Determines whether to continue execution.
	bool processMessage();

	// Sets up and presents a frame which the user draws.
	// The user draws the frame by passing in a function with draw calls.
	void renderFrame(std::function<void(ID3D11DeviceContext*)> drawFrame);

	std::unique_ptr<DirectX::GeometricPrimitive> makeGeometricPrimitive(const DirectX::GeometricPrimitive::VertexCollection&, const DirectX::GeometricPrimitive::IndexCollection&);

	Camera& getCamera();

private:
	Engine();
	~Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	Window window;
	static LRESULT CALLBACK windowProcess(HWND, UINT message, WPARAM, LPARAM);

	// (Re)creates resources which depend on window client size.
	void updateSizeDependentResources(UINT width, UINT height);

	void beginDraw(); // Sets up render surfaces for drawing.
	void endDraw(); // Presents drawn content.

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	Camera camera;
};
