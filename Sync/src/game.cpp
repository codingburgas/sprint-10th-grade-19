#include "game.h"
#include "camera.h"
#include <d3d11.h>
#include <imgui.h>

Game::Game()
	: engine(Engine::getInstance())
	, gameState{State::MainMenu}
	, player{nullptr}
	, floors{nullptr, nullptr, nullptr}
	, mazeWidth{10}
	, mazeHeight{10}
{
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

void Game::beginMazeState()
{
	player = new Player(engine.getCamera(), engine.getKeyboard(), engine.getMouse());
	floors[0] = new Maze(mazeWidth, mazeHeight);
	gameState = State::Maze;
}

void Game::endMazeState()
{
	delete floors[0];
	delete player;
}

void Game::update()
{
	switch (gameState)
	{
	case State::Maze:
		player->update();
	}
}

void Game::draw()
{
	switch (gameState)
	{
	case State::MainMenu:
		engine.renderGui([this]()
		{
			ImGui::SetNextWindowSize(ImGui::GetWindowSize());
			ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoDecoration);
			ImGui::InputScalar("Width", ImGuiDataType_U64, &mazeWidth);
			ImGui::InputScalar("Height", ImGuiDataType_U64, &mazeHeight);
			if (ImGui::Button("Start"))
			{
				beginMazeState();
			}
			ImGui::End();
		});
		break;

	case State::Maze:
		engine.render3DFrame([this](ID3D11DeviceContext*) {

floors[0]->draw(player->getCamera(), 0);
//floors[]
			});
		/*engine.renderGui([this]() {
			ImGui::Begin("Bedug", nullptr, ImGuiWindowFlags_NoDecoration);
			ImGui::Text("%f", player->getCamera().getPos().y);
			ImGui::End();
			});*/
		
		break;
	}
}
