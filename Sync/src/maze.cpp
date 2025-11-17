#include "maze.h"

#include <algorithm>
#include <type_traits>
#include "engine.h"

std::mt19937 Maze::rng(std::random_device{}());
std::uniform_int_distribution<> Maze::pickDirection(static_cast<int>(Maze::Direction::PositiveY), static_cast<int>(Maze::Direction::NegativeX));

std::unique_ptr<DirectX::GeometricPrimitive> Maze::wall;

constexpr Maze::Direction operator|(Maze::Direction lhs, Maze::Direction rhs)
{
	return static_cast<Maze::Direction>(
		static_cast<std::underlying_type<Maze::Direction>::type>(lhs)
		| static_cast<std::underlying_type<Maze::Direction>::type>(rhs)
		);
}

constexpr Maze::Direction operator~(Maze::Direction val)
{
	return static_cast<Maze::Direction>(~static_cast<std::underlying_type<Maze::Direction>::type>(val));
}

constexpr Maze::Cell operator|(Maze::Cell lhs, Maze::Cell rhs)
{
	return static_cast<Maze::Cell>(
		static_cast<std::underlying_type<Maze::Cell>::type>(lhs)
		| static_cast<std::underlying_type<Maze::Cell>::type>(rhs)
		);
}

constexpr Maze::Cell operator|(Maze::Cell cell, Maze::Direction direction)
{
	return cell | static_cast<Maze::Cell>(static_cast<std::underlying_type<Maze::Cell>::type>(direction) << static_cast<std::underlying_type<Maze::Cell>::type>(Maze::Cell::DirectionShift));
}

constexpr Maze::Cell operator&(Maze::Cell cell, Maze::Direction direction)
{
	return static_cast<Maze::Cell>(static_cast<std::underlying_type<Maze::Cell>::type>(cell) & (static_cast<std::underlying_type<Maze::Direction>::type>(direction) << static_cast<std::underlying_type<Maze::Cell>::type>(Maze::Cell::DirectionShift)));
}

constexpr Maze::Cell& operator|=(Maze::Cell& cell, Maze::Direction direction)
{
	return cell = cell | direction;
}

constexpr Maze::Cell& operator&=(Maze::Cell& cell, Maze::Direction direction)
{
	return cell = cell & direction;
}

Maze::Maze(size_t gridWidth, size_t gridHeight)
	: row(std::clamp(gridHeight, static_cast<size_t>(10), static_cast<size_t>(50)))
	, col(std::clamp(gridWidth, static_cast<size_t>(10), static_cast<size_t>(50)))
{
	maze = new Cell*[row];
	for (size_t i = 0; i < row; i++)
	{
		maze[i] = new Cell[col];
	}
	for (size_t i = 0; i < row; i++)
	{
		for (size_t j = 0; j < col - 1; j++)
		{
			maze[i][j] = Cell::AllWalls | Direction::PositiveX;
		}
		maze[i][col - 1] = Cell::AllWalls | Direction::NegativeY;
	}
	maze[row - 1][col - 1] = Cell::AllWalls;
	generateMaze(maze, row, col, row - 1, col - 1);

	DirectX::GeometricPrimitive::VertexCollection wallVertices;
	DirectX::GeometricPrimitive::IndexCollection wallIndices;
	DirectX::GeometricPrimitive::CreateBox(wallVertices, wallIndices, wallDimensions);
	wall = Engine::getInstance().makeGeometricPrimitive(wallVertices, wallIndices);
}

Maze::~Maze()
{
	for (size_t i = 0; i < row; i++)
	{
		delete[] maze[i];
	}
	delete[] maze;
}

void Maze::generateMaze(Cell** maze, size_t row, size_t col, size_t originRow, size_t originCol)
{
	Direction randomDirection;

	for (size_t i = 0; i < row * col * 10; i++)
	{
		randomDirection = static_cast<Direction>(pickDirection(rng));

		if (isNewOriginOutOfBounds(randomDirection, row, col, originRow, originCol))
		{
			continue;
		}

		switch (randomDirection)
		{
		case Direction::PositiveY:
			maze[originRow][originCol] |= Direction::PositiveY;
			originRow--;
			maze[originRow][originCol] &= ~Maze::Direction::Mask;
			break;

		case Direction::PositiveX:
			maze[originRow][originCol] |= Direction::PositiveX;
			originCol++;
			maze[originRow][originCol] &= ~Maze::Direction::Mask;
			break;

		case Direction::NegativeY:
			maze[originRow][originCol] |= Direction::NegativeY;
			originRow++;
			maze[originRow][originCol] &= ~Maze::Direction::Mask;
			break;

		case Direction::NegativeX:
			maze[originRow][originCol] |= Direction::NegativeX;
			originCol--;
			maze[originRow][originCol] &= ~Maze::Direction::Mask;
			break;
		}
	}

	for (size_t i = 0; i < row; i++)
	{
		for (size_t j = 0; j < col; j++)
		{
			// Set wall flags
		}
	}
}

bool Maze::isNewOriginOutOfBounds(Direction direction, size_t row, size_t col, size_t originRow, size_t originCol)
{
	switch (direction)
	{
	case Direction::PositiveY: return originRow - 1 > 0;
	case Direction::PositiveX: return originCol + 1 < col;
	case Direction::NegativeY: return originRow + 1 < row;
	case Direction::NegativeX: return originCol - 1 > 0;
	}
}
