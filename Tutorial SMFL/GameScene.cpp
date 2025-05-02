#include "GameScene.h"
#include "GameManager.h"

void GameScene::enter(sf::RenderWindow& window)
{
	std::cout << "Entras a la escena del juego" << std::endl;
	GAME.Init(new Map(window));

}

void GameScene::exit()
{
	std::cout << "Sales del juego" << std::endl;
}

void GameScene::update(sf::RenderWindow& window, const sf::Event& event)
{
	GAME.Update(window, event);
}

void GameScene::DetectRectangle(sf::Vector2f mousePosition)
{
}
