#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <GeometricPrimitive.h>//
#include <wrl/client.h>
#include "window.h"
#include "camera.h"//

class Engine
{
public:
	static Engine& getInstance();

	bool processMessage();

	void renderFrame();

private:
	Engine();
	~Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	Window window;
	static LRESULT CALLBACK windowProcess(HWND, UINT message, WPARAM, LPARAM);

	// (Re)creates resources which depend on window client size.
	void updateSizeDependentResources(UINT width, UINT height);

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	std::unique_ptr<DirectX::GeometricPrimitive> shape;
	DirectX::SimpleMath::Matrix shapeWorldMatrix;
	Camera* camera = nullptr;
};
