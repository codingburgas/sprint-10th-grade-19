#pragma once

#include "engine.h"
#include "player.h"

class Game
{
public:
	static void begin();

private:
	Game();
	~Game() = default;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void update();
	void draw();

	Engine& engine;
	Player player;
	std::unique_ptr<DirectX::GeometricPrimitive> wall;
};
