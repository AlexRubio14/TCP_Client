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

	currentPlayerIndex = 0;
	currentPlayer = players[currentPlayerIndex];
	StartTurn();
}

void GameManager::Update(sf::RenderWindow& window, const sf::Event& event)
{
	window.clear();

	map->Update(window);
	for (Player* player : players)
	{
		player->Update(window);
	}
	HandleEvent(event, window);

	currentPlayer->HandleEvent(event, window);

	window.display();
}

void GameManager::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	if (event.is<sf::Event::Closed>())
		window.close();

	//Aqui hay que hacer que se desconecte el jugador 
}

void GameManager::StartTurn()
{
	currentPlayer->SetCanThrowDice(true);
}

void GameManager::EndTurn()
{
	currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
	currentPlayer = players[currentPlayerIndex];
	StartTurn();
}

Token* GameManager::TokenInPosition(Cell* currentCell)
{
	for (Player* player : players)
	{
		if (player == currentPlayer)
			continue;

		for (Token* token : player->GetTokens())
		{
			if (token->GetCurrentCell() == currentCell)
				return token;
		}
	}

	return nullptr;
}
