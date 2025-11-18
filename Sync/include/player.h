#pragma once

#include <Mouse.h>
#include <DirectXMath.h>

class Camera;

class Player
{
public:
	Player(Camera&);

	void update();

	DirectX::CXMMATRIX getViewMatrix();
	DirectX::CXMMATRIX getProjectionMatrix();

private:
	Camera& camera;

	DirectX::Mouse& mouse;
};
