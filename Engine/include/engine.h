#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>
#include <Effects.h>
#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include "window.h"

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
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	DirectX::BasicEffect* effect = nullptr;
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch = nullptr;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
};
