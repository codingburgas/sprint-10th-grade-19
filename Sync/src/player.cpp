#include "player.h"
#include "camera.h"
#include <system_error>

Player::Player(Camera& camera, DirectX::Keyboard& keyboard, DirectX::Mouse& mouse)
	: camera(camera)
	, keyboard(keyboard)
	, mouse(mouse)
{
	camera.setPosition({ -2, 2, -2 });
		mouse.SetMode(DirectX::Mouse::MODE_RELATIVE);


	
}

void Player::update()
{
	DirectX::Mouse::State mouseState = mouse.GetState();
	DirectX::Keyboard::State keyboardState = keyboard.GetState();

	if (mouseState.x != 0)
	{
		camera.adjustYaw(mouseState.x);
	}
	if (keyboardState.W)
	{
		camera.adjustPosition({0, 0, -1 * 0.0007}, 0);
	}
	if (keyboardState.A)
	{
		camera.adjustPosition({-1 * 0.0007, 0, 0}, 0);
	}
	if (keyboardState.Space)
	{
		camera.adjustPosition({0, 1 * 0.0007, 0}, 1);
	}
	if (keyboardState.D)
	{
		camera.adjustPosition({1 * 0.0007, 0, 0}, 0);
	}
}

const Camera& Player::getCamera() const
{
	return camera;
}
