#include "engine.h"
#include <DirectXColors.h>
#include <DirectXHelpers.h>

Engine::Engine()
	: window(inputStream)
{
	Window::ClientSize clientSize = window.getClientSize();

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
		nullptr, // not using software rasterizer
		D3D11_CREATE_DEVICE_SINGLETHREADED,
		nullptr, // use default feature levels
		0, // custom feature level count
		D3D11_SDK_VERSION,
		&swapChainDescription,
		swapChain.ReleaseAndGetAddressOf(),
		device.ReleaseAndGetAddressOf(),
		nullptr, // outChosenFeatureLevel
		deviceContext.ReleaseAndGetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.ReleaseAndGetAddressOf()));

	device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.ReleaseAndGetAddressOf());

	effect = new DirectX::BasicEffect(device.Get());
	effect->SetVertexColorEnabled(true);
	effect->Apply(deviceContext.Get());

	DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(device.Get(), effect, inputLayout.ReleaseAndGetAddressOf());
	deviceContext->IASetInputLayout(inputLayout.Get());

	D3D11_VIEWPORT viewport{0, 0, clientSize.width, clientSize.height, 0, 1};
	deviceContext->RSSetViewports(1, &viewport);

	D3D11_TEXTURE2D_DESC depthStencilBufferDescription{};
	depthStencilBufferDescription.Width = clientSize.width;
	depthStencilBufferDescription.Height = clientSize.height;
	depthStencilBufferDescription.MipLevels = 1;
	depthStencilBufferDescription.ArraySize = 1;
	depthStencilBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDescription.SampleDesc.Count = 1;
	depthStencilBufferDescription.SampleDesc.Quality = 0;
	depthStencilBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	D3D11_DEPTH_STENCIL_DESC depthStencilDescription{};
	depthStencilDescription.DepthEnable = true;
	depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateTexture2D(&depthStencilBufferDescription, nullptr, depthStencilBuffer.ReleaseAndGetAddressOf());
	device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.ReleaseAndGetAddressOf());
	device->CreateDepthStencilState(&depthStencilDescription, depthStencilState.ReleaseAndGetAddressOf());

	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

	primitiveBatch = new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(deviceContext.Get());
}

Engine::~Engine()
{
	delete primitiveBatch;
	delete effect;
}

bool Engine::processMessage()
{
	return window.dispatchMessage();
}

void Engine::renderFrame()
{
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	primitiveBatch->Begin();
	primitiveBatch->DrawTriangle(
		{{0, 0.2, 1}, DirectX::Colors::Lavender},
		{{0.2, 0, 1}, DirectX::Colors::Lavender},
		{{-0.2, 0, 1}, DirectX::Colors::Lavender}
	);
	primitiveBatch->DrawTriangle(
		{{0, 0.3, 0}, DirectX::Colors::LawnGreen},
		{{0.1, 0.1, 0}, DirectX::Colors::LawnGreen},
		{{-0.1, 0.1, 0}, DirectX::Colors::LawnGreen}
	);
	primitiveBatch->End();

	swapChain->Present(0, 0);
}
