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

	D3D11_VIEWPORT viewport{0, 0, clientSize.width, clientSize.height};
	deviceContext->RSSetViewports(1, &viewport);

	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

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
	static DirectX::VertexPositionColor vertices[] =
	{
		{{0, 0, 0}, DirectX::Colors::LawnGreen},
		{{-0.1, 0, 0}, DirectX::Colors::LawnGreen}
	};

	deviceContext->ClearRenderTargetView(renderTargetView.Get(), DirectX::Colors::CornflowerBlue);

	primitiveBatch->Begin();
	primitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, vertices, 2);
	primitiveBatch->End();
	
	swapChain->Present(0, 0);
}
