#include "GameScene.h"
#include "GameManager.h"

void GameScene::enter(sf::RenderWindow& window)
{
	std::cout << "Enter Game Scene" << std::endl;
	GAME.Init(new Map(window));

}

void GameScene::exit()
{
	std::cout << "Exit Game Scene" << std::endl;
}

void GameScene::update(sf::RenderWindow& window, const sf::Event& event)
{
	GAME.Update(window, event);
}

void GameScene::DetectRectangle(sf::Vector2f mousePosition)
{
}
