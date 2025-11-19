#pragma once

#include <GeometricPrimitive.h>
#include <DirectXMath.h>
#include <random>
#include <type_traits>
#include <cstdint>

// 3D maze using origin shift algorithm for generation.
class Maze
{
public:
	Maze(size_t gridWidth, size_t gridHeight);
	~Maze();

	void draw();

private:
	size_t row, col;

	struct Cell
	{
		// Wall flags for a maze cell.
		enum class Walls
		{
			Top    = 1 << 0,
			Right  = 1 << 1,
			Bottom = 1 << 2,
			Left   = 1 << 3,

			AllWalls = Top | Right | Bottom | Left
		} walls;

		enum class Direction
		{
			None,
			Up,
			Down,
			Left,
			Right
		} nextCell;
	}** maze;

	// Using the origin shift algorithm, generate a new maze given an initialized perfect maze.
	// The origin is one cell which doesn't point to any other cells.
	static void generateMaze(Cell** maze, size_t row, size_t col, size_t originRow, size_t originCol);
	static bool isNewOriginOutOfBounds(Cell::Direction nextCell, size_t row, size_t col, size_t originRow, size_t originCol);
	static Cell::Walls getWallFacingDirection(Cell::Direction);
	static Cell::Walls getWallOppositeDirection(Cell::Direction);
	static Cell& getCellOffsetByDirection(Cell::Direction, Cell** maze, size_t i, size_t j);
	static std::mt19937 rng;
	static std::uniform_int_distribution<> pickDirection;

	const DirectX::XMFLOAT3 wallDimensions = {4, 3, 0};
	std::unique_ptr<DirectX::GeometricPrimitive> wall;

	friend constexpr Cell::Walls operator~(Cell::Walls);
	friend constexpr Cell::Walls operator&(Cell::Walls, Cell::Walls);
	friend constexpr Cell::Walls& operator&=(Cell::Walls&, Cell::Walls);
};

constexpr Maze::Cell::Walls operator~(Maze::Cell::Walls val)
{
	using WallsInt = std::underlying_type_t<Maze::Cell::Walls>;

	return static_cast<Maze::Cell::Walls>(~static_cast<WallsInt>(val));
}

constexpr Maze::Cell::Walls operator&(Maze::Cell::Walls lhs, Maze::Cell::Walls rhs)
{
	using WallsInt = std::underlying_type_t<Maze::Cell::Walls>;

	return static_cast<Maze::Cell::Walls>(
		static_cast<WallsInt>(rhs) & static_cast<WallsInt>(lhs)
	);
}

constexpr Maze::Cell::Walls& operator&=(Maze::Cell::Walls& lhs, Maze::Cell::Walls rhs)
{
	return lhs = lhs & rhs;
}
