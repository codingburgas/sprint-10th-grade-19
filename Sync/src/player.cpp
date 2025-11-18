#include "player.h"
#include "camera.h"
#include <Mouse.h>

Player::Player(Camera& camera)
	: camera(camera)
	, mouse(DirectX::Mouse::Get())
{
	mouse.SetMode(DirectX::Mouse::MODE_RELATIVE);
}

void Player::update()
{
	DirectX::Mouse::State mouseState = mouse.GetState();

	if (mouseState.x != 0)
	{
		camera.adjustYaw(mouseState.x);
	}
}

DirectX::CXMMATRIX Player::getViewMatrix()
{
	return camera.getViewMatrix();
}

DirectX::CXMMATRIX Player::getProjectionMatrix()
{
	return camera.getProjectionMatrix();
}
