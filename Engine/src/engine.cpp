#include "engine.h"
#include <DirectXColors.h>
#include <DirectXHelpers.h>

Engine::Engine()
{
	Window::ClientSize clientSize = window.getInitialClientSize();

	DXGI_SWAP_CHAIN_DESC swapChainDescription{};
	swapChainDescription.BufferDesc.Width = clientSize.width;
	swapChainDescription.BufferDesc.Height = clientSize.height;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.BufferCount = 1;
	swapChainDescription.OutputWindow = window.getHandle();
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(
		nullptr, // use default video adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, // use hardware rasterizer
		D3D11_CREATE_DEVICE_SINGLETHREADED,
		nullptr, // use default feature levels
		0, // custom feature level count
		D3D11_SDK_VERSION,
		&swapChainDescription,
		swapChain.ReleaseAndGetAddressOf(),
		device.ReleaseAndGetAddressOf(),
		nullptr, // outChosenFeatureLevel
		deviceContext.ReleaseAndGetAddressOf()
	);

	camera = new Camera(clientSize.width, clientSize.height);
	updateSizeDependentResources(clientSize.width, clientSize.height);
	window.overrideWindowProcess(windowProcess);

	D3D11_DEPTH_STENCIL_DESC depthStencilDescription{};
	depthStencilDescription.DepthEnable = true;
	depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&depthStencilDescription, depthStencilState.ReleaseAndGetAddressOf());
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

	shape = DirectX::GeometricPrimitive::CreateBox(deviceContext.Get(), {2, 3, 1});
	shapeWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld({0, 0, -5}, DirectX::SimpleMath::Vector3::Forward, DirectX::SimpleMath::Vector3::Up);
}

Engine::~Engine()
{
	delete camera;
}

void Engine::updateSizeDependentResources(UINT width, UINT height)
{
	renderTargetView.Reset();
	depthStencilView.Reset();

	swapChain->ResizeBuffers(
		0, // keep existing count
		width, height,
		DXGI_FORMAT_UNKNOWN, // keep existing format
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.ReleaseAndGetAddressOf()));
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.ReleaseAndGetAddressOf());

	D3D11_TEXTURE2D_DESC depthStencilBufferDescription{};
	depthStencilBufferDescription.Width = width;
	depthStencilBufferDescription.Height = height;
	depthStencilBufferDescription.MipLevels = 1;
	depthStencilBufferDescription.ArraySize = 1;
	depthStencilBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDescription.SampleDesc.Count = 1;
	depthStencilBufferDescription.SampleDesc.Quality = 0;
	depthStencilBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	device->CreateTexture2D(&depthStencilBufferDescription, nullptr, depthStencilBuffer.ReleaseAndGetAddressOf());
	device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.ReleaseAndGetAddressOf());
	
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	D3D11_VIEWPORT viewport{0, 0, width, height, 0, 1};
	deviceContext->RSSetViewports(1, &viewport);

	camera->updateAspectRatio(width, height);
}

void Engine::renderFrame()
{
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	shape->Draw(shapeWorldMatrix, camera->getViewMatrix(), camera->getProjectionMatrix());

	swapChain->Present(0, 0);
}

LRESULT Engine::windowProcess(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		Engine::getInstance().updateSizeDependentResources(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	return DefWindowProc(windowHandle, message, wParam, lParam);
}

Engine& Engine::getInstance()
{
	static Engine engine;

	return engine;
}

bool Engine::processMessage()
{
	return window.dispatchMessage();
}
