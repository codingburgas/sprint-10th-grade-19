#include "game.h"
#include "camera.h"
#include <d3d11.h>
#include <imgui.h>

Game::Game()
	: engine(Engine::getInstance())
	, player(engine.getCamera())
	, gameState(State::MainMenu)
	, mazeWidth(10)
	, mazeHeight(10)
{
	DirectX::GeometricPrimitive::VertexCollection wallVertices;
	DirectX::GeometricPrimitive::IndexCollection wallIndices;
	DirectX::GeometricPrimitive::CreateBox(wallVertices, wallIndices, { 4, 3, 0 });
	wall = Engine::getInstance().makeGeometricPrimitive(wallVertices, wallIndices);
	ImGui::GetIO().Fonts->AddFontFromFileTTF("resources\\Fruktur-Regular.ttf");
}

void Game::begin()
{
	Game game;

	while (game.engine.processMessages())
	{
		game.update();
		game.draw();
	}
}

void Game::update()
{
	player.update();
}

void Game::draw()
{
	switch (gameState)
	{
	case State::MainMenu:
		engine.renderGui([]()
		{
			ImGui::SetWindowSize(ImGui::GetWindowSize());
			ImGui::Begin("", nullptr, ImGuiWindowFlags_NoDecoration);
			if (ImGui::Button("Start"))
			{
				maze = Maze(width, height);
				gameState = State::Maze;
			}
			ImGui::End();
		});
		break;

	case State::Maze:
		maze.draw();
		break;
	}
}
