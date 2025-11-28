#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include <DirectXMath.h>

class Camera;

class Player
{
public:
	Player(Camera&, DirectX::Keyboard&, DirectX::Mouse&);

	void update();

	const Camera& getCamera() const;

private:
	Camera& camera;
	DirectX::Keyboard& keyboard;
	DirectX::Mouse& mouse;
};
