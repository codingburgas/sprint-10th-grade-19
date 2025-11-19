#include "maze.h"
#include "engine.h"
#include <algorithm>

std::mt19937 Maze::rng(std::random_device{}());
std::uniform_int_distribution<> Maze::pickDirection(static_cast<int>(Maze::Cell::Direction::Up), static_cast<int>(Maze::Cell::Direction::Right));

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
			maze[i][j].walls = Cell::Walls::AllWalls;
			maze[i][j].nextCell = Cell::Direction::Right;
		}
		maze[i][col - 1].walls = Cell::Walls::AllWalls;
		maze[i][col - 1].nextCell = Cell::Direction::Down;
	}
	maze[row - 1][col - 1].nextCell = Cell::Direction::None;
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
	Cell::Direction randomDirection;

	for (size_t i = 0; i < row * col * 10; i++)
	{
		randomDirection = static_cast<Cell::Direction>(pickDirection(rng));

		if (isNewOriginOutOfBounds(randomDirection, row, col, originRow, originCol))
		{
			continue;
		}

		maze[originRow][originCol].nextCell = randomDirection;
		switch (randomDirection)
		{
		case Cell::Direction::Up:
			originRow--;
			break;

		case Cell::Direction::Down:
			originRow++;
			break;

		case Cell::Direction::Left:
			originCol--;
			break;

		case Cell::Direction::Right:
			originCol++;
			break;
		}
		maze[originRow][originCol].nextCell = Cell::Direction::None;
	}

	for (size_t i = 0; i < row; i++)
	{
		for (size_t j = 0; j < col; j++)
		{
			maze[i][j].walls &= ~getWallFacingDirection(maze[i][j].nextCell);
			getCellOffsetByDirection(maze[i][j].nextCell, maze, i, j).walls &= ~getWallOppositeDirection(maze[i][j].nextCell);
		}
	}
}

bool Maze::isNewOriginOutOfBounds(Cell::Direction nextCell, size_t row, size_t col, size_t originRow, size_t originCol)
{
	switch (nextCell)
	{
	case Cell::Direction::Up:
		return originRow <= 0;

	case Cell::Direction::Right:
		return originCol + 1 >= col;

	case Cell::Direction::Down:
		return originRow + 1 >= row;

	case Cell::Direction::Left:
		return originCol <= 0;
	}
}

Maze::Cell& Maze::getCellOffsetByDirection(Cell::Direction direction, Cell** maze, size_t i, size_t j)
{
	switch (direction)
	{
	case Cell::Direction::Up:
		return maze[i - 1][j];

	case Cell::Direction::Down:
		return maze[i + 1][j];

	case Cell::Direction::Left:
		return maze[i][j - 1];

	case Cell::Direction::Right:
		return maze[i][j + 1];
	}
}

Maze::Cell::Walls Maze::getWallFacingDirection(Cell::Direction direction)
{
	switch (direction)
	{
	case Cell::Direction::Up:
		return Cell::Walls::Top;

	case Cell::Direction::Down:
		return Cell::Walls::Bottom;

	case Cell::Direction::Left:
		return Cell::Walls::Left;

	case Cell::Direction::Right:
		return Cell::Walls::Right;
	}
}

Maze::Cell::Walls Maze::getWallOppositeDirection(Cell::Direction direction)
{
	switch (direction)
	{
	case Cell::Direction::Up:
		return Cell::Walls::Bottom;

	case Cell::Direction::Down:
		return Cell::Walls::Top;

	case Cell::Direction::Right:
		return Cell::Walls::Left;

	case Cell::Direction::Left:
		return Cell::Walls::Right;
	}
}
