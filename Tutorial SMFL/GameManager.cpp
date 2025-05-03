#include "GameManager.h"
#include "TimeManager.h"
#include <iostream>

void GameManager::Init(sf::RenderWindow& _window)
{
	map = new Map(_window);

	currentClientIndex = 0;
	currentClient = clients[currentClientIndex];

	for (int i = 0; i < clients.size(); i++)
		map->SetName(i, clients[i]->GetName());

	StartTurn();
}

void GameManager::Update(sf::RenderWindow& window, const sf::Event& event)
{
	if (currentClient == nullptr)
		return;

	if (TIME.IsTurnTimeOver())
	{
		std::cout << "Se acabó el tiempo, cambio de turno";
		EndTurn();
	}

	window.clear();

	map->Update(window);

	for (Client* client : clients)
	{
		client->Update(window);
	}
	HandleEvent(event, window);

	currentClient->HandleEvent(event, window);

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
	currentClient->SetCanThrowDice(true);
	currentClient->ResetDiceValue();
	TIME.StartTurn();
}

void GameManager::EndTurn()
{
	currentClientIndex = (currentClientIndex + 1) % clients.size();
	currentClient = clients[currentClientIndex];
	StartTurn();
}

Token* GameManager::TokenInPosition(Token* tokenChecked)
{
	for (Client* client : clients)
	{
		for (Token* token : client->GetTokens())
		{
			if (token->GetCurrentCell() == tokenChecked->GetCurrentCell() && token != tokenChecked)
				return token;
		}
	}

	return nullptr;
}

void GameManager::AddClient(const std::string &ip, const std::string &name, const int &index)
{
	sf::Color color;
	if (clients.size() < 1)
		color = sf::Color::Red;
	else if (clients.size() < 2)
		color = sf::Color::Green;
	else if (clients.size() < 3)
		color = sf::Color::Blue;
	else
		color = sf::Color::Yellow;

	Client* newClient = new Client(ip, name, color, index);
	clients.push_back(newClient);
}
