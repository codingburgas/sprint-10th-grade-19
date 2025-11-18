#include "game.h"
#include "camera.h"

Game::Game()
	: engine(Engine::getInstance())
	, player(engine.getCamera())
{
	DirectX::GeometricPrimitive::VertexCollection wallVertices;
	DirectX::GeometricPrimitive::IndexCollection wallIndices;
	DirectX::GeometricPrimitive::CreateBox(wallVertices, wallIndices, { 4, 3, 0 });
	wall = Engine::getInstance().makeGeometricPrimitive(wallVertices, wallIndices);
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
	engine.renderFrame([this]()
	{
		wall->Draw(DirectX::SimpleMath::Matrix::CreateWorld({ 0, 0, -2 }, { 0, 0, -1 }, { 0, 1, 0 }), player.getViewMatrix(), player.getProjectionMatrix());
	});
}
