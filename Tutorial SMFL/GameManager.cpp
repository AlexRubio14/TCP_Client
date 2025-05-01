#include "GameManager.h"

void GameManager::Init(Map* _map)
{
	map = _map;

	Player* player1 = new Player("Crami", sf::Color::Red);
	Player* player2 = new Player("Gemix", sf::Color::Green);
	Player* player3 = new Player("Alex", sf::Color::Blue);
	Player* player4 = new Player("Fran", sf::Color::Yellow);
	players = std::vector<Player*>();
	players.push_back(player1);
	players.push_back(player2);
	players.push_back(player3);
	players.push_back(player4);
}

void GameManager::Update(sf::RenderWindow& window)
{
	map->Update(window);
	for (Player* player : players)
	{
		player->Update(window);
	}
}
