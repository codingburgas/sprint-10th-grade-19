#pragma once

#include "engine.h"
#include "player.h"
#include "maze.h"

class Game
{
public:
	static void begin();

private:
	Game();
	~Game() = default;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	enum class State
	{
		MainMenu,
		Maze
	} gameState;

	void update();
	void draw();

	Engine& engine;
	Player player;
	Maze maze;

	size_t mazeWidth, mazeHeight;
};
