#include "GameManager.h"
#include "TimeManager.h"
#include <iostream>

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
	if (TIME.IsTurnTimeOver())
	{
		std::cout << "Se acabó el tiempo, cambio de turno";
		EndTurn();
	}

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
	TIME.StartTurn();
}

void GameManager::EndTurn()
{
	currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
	currentPlayer = players[currentPlayerIndex];
	StartTurn();
}

Token* GameManager::TokenInPosition(Token* tokenChecked)
{
	for (Player* player : players)
	{
		for (Token* token : player->GetTokens())
		{
			if (token->GetCurrentCell() == tokenChecked->GetCurrentCell() && token != tokenChecked)
				return token;
		}
	}

	return nullptr;
}
