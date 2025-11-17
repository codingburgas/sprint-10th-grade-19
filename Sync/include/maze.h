#pragma once

#include <cstdint>
#include <GeometricPrimitive.h>
#include <random>
#include <SimpleMath.h>

// 3D maze using origin shift algorithm for generation.
class Maze
{
public:
	Maze(size_t gridWidth, size_t gridHeight);
	~Maze();

	void drawMaze();

	// Possible walls for a grid cell.
	// Can store a direction which points to the next cell in the maze.
	enum class Cell : uint8_t
	{
		Top    = 1 << 0,
		Right  = 1 << 1,
		Bottom = 1 << 2,
		Left   = 1 << 3,
		AllWalls = Top | Right | Bottom | Left,

		DirectionShift = 4
	};
	enum class Direction : uint8_t
	{
		PositiveY = 1,
		PositiveX = 2,
		NegativeY = 3,
		NegativeX = 4,
		Mask = PositiveY | PositiveX | NegativeY | NegativeX
	};

private:
	size_t row, col;
	Cell** maze;

	// Using the origin shift algorithm, generate a new maze given an initialized perfect maze.
	// The origin is one cell which doesn't point to any other cells.
	static void generateMaze(Cell** maze, size_t row, size_t col, size_t originRow, size_t originCol);
	static bool isNewOriginOutOfBounds(Direction direction, size_t row, size_t col, size_t originRow, size_t originCol);
	static std::mt19937 rng;
	static std::uniform_int_distribution<> pickDirection;

	const DirectX::XMFLOAT3 wallDimensions = {4, 3, 0};
	static std::unique_ptr<DirectX::GeometricPrimitive> wall;
};
