#include "maze.h"
#include "camera.h"
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

	DirectX::GeometricPrimitive::VertexCollection vertices;
	DirectX::GeometricPrimitive::IndexCollection indices;

	DirectX::GeometricPrimitive::CreateBox(vertices, indices, wallDimensions);
	wall = Engine::getInstance().makeGeometricPrimitive(vertices, indices);
	
	DirectX::GeometricPrimitive::CreateBox(vertices, indices, floorDimensions);
	floor = Engine::getInstance().makeGeometricPrimitive(vertices, indices);
	
	Engine::getInstance().makeTexture(L"resources\\wall.jpg", wallTexture.ReleaseAndGetAddressOf());
	Engine::getInstance().makeTexture(L"resources\\floor.jpg", floorTexture.ReleaseAndGetAddressOf());
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

		if (isDirectionOutOfBounds(randomDirection, row, col, originRow, originCol))
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

void Maze::draw(const Camera& camera, int floorNumber)
{
	DirectX::SimpleMath::Matrix wallWorldMatrix;
	DirectX::SimpleMath::Matrix floorWorldMatrix;

	for (int i = 0; i < col; i++)
	{
		wallWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
			{-i * wallDimensions.x - wallDimensions.x / 2, wallDimensions.y / 2, 0},
			DirectX::SimpleMath::Vector3::Forward,
			DirectX::SimpleMath::Vector3::Up
		);
		wall->Draw(wallWorldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix(), DirectX::SimpleMath::Color(3, 3, 3), wallTexture.Get());
	}
	for (int i = 1; i < row - 1; i++)
	{
		wallWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
			{0, wallDimensions.y / 2, -i * wallDimensions.x - wallDimensions.x / 2},
			DirectX::SimpleMath::Vector3::Left,
			DirectX::SimpleMath::Vector3::Up
		);
		wall->Draw(wallWorldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix(), DirectX::SimpleMath::Color(3, 3, 3), wallTexture.Get());
	}
	for (int i = 0; i < col - 1; i++)
	{
		wallWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
			{ -i * wallDimensions.x - wallDimensions.x / 2, wallDimensions.y / 2, -(int)row * wallDimensions.x},
			DirectX::SimpleMath::Vector3::Backward,
			DirectX::SimpleMath::Vector3::Up
		);
		wall->Draw(wallWorldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix(), DirectX::SimpleMath::Color(3, 3, 3), wallTexture.Get());
	}
	wallWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
		{ -int(col-1) * wallDimensions.x - wallDimensions.x / 2, wallDimensions.y / 2, -(int)row * wallDimensions.x },
		DirectX::SimpleMath::Vector3::Backward,
		DirectX::SimpleMath::Vector3::Up
	);
	wall->Draw(wallWorldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix(), DirectX::Colors::Green, wallTexture.Get());

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i == row - 1 && j == col - 1)
				break;
			floorWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
				{ -j * floorDimensions.x - floorDimensions.x / 2, 0, -i * floorDimensions.y - floorDimensions.y / 2 },
				DirectX::SimpleMath::Vector3::Up,
				DirectX::SimpleMath::Vector3::Forward
			);
			floor->Draw(floorWorldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix(), DirectX::Colors::White, floorTexture.Get());
		}
	}
	floorWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
		{ -int(col - 1) * floorDimensions.x - floorDimensions.x / 2, 0, -int(row-1) * floorDimensions.y - floorDimensions.y / 2 },
		DirectX::SimpleMath::Vector3::Up,
		DirectX::SimpleMath::Vector3::Forward
	);
	floor->Draw(floorWorldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix(), DirectX::Colors::Green, floorTexture.Get());


	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				switch (maze[i][j].walls & static_cast<Cell::Walls>(1 << k))
				{
				case Cell::Walls::Top:
					wallWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
						{-j * wallDimensions.x - wallDimensions.x / 2, wallDimensions.y / 2, -i * wallDimensions.x},
						DirectX::SimpleMath::Vector3::Forward,
						DirectX::SimpleMath::Vector3::Up
					);
					break;

				case Cell::Walls::Bottom:
					wallWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
						{-j * wallDimensions.x - wallDimensions.x / 2, wallDimensions.y / 2, (-j - 1) * wallDimensions.x},
						DirectX::SimpleMath::Vector3::Backward,
						DirectX::SimpleMath::Vector3::Up
					);
					break;

				case Cell::Walls::Left:
					wallWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
						{-j * wallDimensions.x, wallDimensions.y / 2, -i * wallDimensions.x - wallDimensions.x / 2},
						DirectX::SimpleMath::Vector3::Left,
						DirectX::SimpleMath::Vector3::Up
					);
					break;

				case Cell::Walls::Right:
					wallWorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
						{(-j - 1) * wallDimensions.x, wallDimensions.y / 2, -i * wallDimensions.x - wallDimensions.x / 2},
						DirectX::SimpleMath::Vector3::Right,
						DirectX::SimpleMath::Vector3::Up
					);
					break;
				}
				wall->Draw(wallWorldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix(),DirectX::SimpleMath::Color(3, 3, 3), wallTexture.Get());
			}
		}
	}
}

bool Maze::isDirectionOutOfBounds(Cell::Direction nextCell, size_t row, size_t col, size_t i, size_t j)
{
	switch (nextCell)
	{
	case Cell::Direction::Up:
		return i == 0;

	case Cell::Direction::Down:
		return i + 1 >= row;

	case Cell::Direction::Left:
		return j == 0;

	case Cell::Direction::Right:
		return j + 1 >= col;
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
