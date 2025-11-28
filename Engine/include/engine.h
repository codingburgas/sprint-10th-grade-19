#pragma once

#include "camera.h"
#include "window.h"
#include <GeometricPrimitive.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <functional>

class Engine
{
public:
	static Engine& getInstance();

	// Processes window input. Determines whether to continue execution.
	bool processMessages();

	// Sets up and presents a 3D frame which the user draws.
	// The user draws the frame by passing in a function with draw calls.
	void render3DFrame(std::function<void(ID3D11DeviceContext*)> draw3DFrame);

	// Sets up and presents a 2D GUI which the user draws.
	// The user has to pass in a function with draw calls.
	void renderGui(std::function<void()> drawGui);

	DirectX::Keyboard& getKeyboard();
	DirectX::Mouse& getMouse();
	void makeTexture(LPCWSTR filepath, ID3D11ShaderResourceView** texture);
	std::unique_ptr<DirectX::GeometricPrimitive> makeGeometricPrimitive(const DirectX::GeometricPrimitive::VertexCollection&, const DirectX::GeometricPrimitive::IndexCollection&);

	Camera& getCamera();

private:
	Engine();
	~Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	Window window;
	static LRESULT CALLBACK windowProcess(HWND, UINT message, WPARAM, LPARAM);

	// (Re)creates resources which depend on window client size.
	void updateSizeDependentResources(UINT width, UINT height);

	void clearViews(); // Makes sure views are empty before drawing.
	void presentDrawnFrame(); // Presents drawn content.

	void beginGuiDraw(); // Sets up frame for Dear ImGui drawing.
	void endGuiDraw(); // Presents the drawn frame.

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	DirectX::Keyboard* keyboard = nullptr;
	DirectX::Mouse* mouse = nullptr;

	Camera camera;
};
