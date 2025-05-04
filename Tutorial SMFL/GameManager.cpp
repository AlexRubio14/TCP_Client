#include "GameManager.h"
#include "TimeManager.h"
#include <iostream>
#include "NetworkManager.h"
#include "EventManager.h"

void GameManager::Init(sf::RenderWindow& _window)
{
	map = std::make_unique<Map>(_window);
}

void GameManager::Update(sf::RenderWindow& window, const sf::Event& event)
{

	if (currentClient == nullptr)
		return;

	//if (TIME.IsTurnTimeOver())
	//{
	//	std::cout << "Se acabó el tiempo, cambio de turno";
	//	EndTurn();
	//}

	if(referenceClient->GetIndex() == currentClient->GetIndex())
		currentClient->HandleEvent(event, window);
	HandleEvent(event, window);

	window.clear();

	map->Update(window);

	for (const std::shared_ptr<Client>& client : clients)
	{
		client->Update(window);
	}

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
	CustomPacket packet;
	EVENT_MANAGER.Emit(END_TURN, " ", packet);
	currentClientIndex = (currentClientIndex + 1) % clients.size();
	currentClient = clients[currentClientIndex];
	StartTurn();
}

void GameManager::StartGame()
{
	currentClientIndex = 0;
	currentClient = clients[currentClientIndex];
	StartTurn();
}

const std::shared_ptr<Token>& GameManager::TokenInPosition(Token* tokenChecked)
{
	for (const std::shared_ptr<Client>& client : clients)
	{
		for (const std::shared_ptr<Token>& token : client->GetTokens())
		{
			if (token->GetCurrentCell() == tokenChecked->GetCurrentCell() && token.get() != tokenChecked)
				return token;
		}
	}

	return nullptr;
}

void GameManager::AddClient(const std::string &ip, const std::string &name, const int &index, const int& numPort)
{
	sf::Color color;
	std::cout << clients.size() << std::endl;
	if (clients.size() < 1)
		color = sf::Color::Red;
	else if (clients.size() < 2)
		color = sf::Color::Green;
	else if (clients.size() < 3)
		color = sf::Color::Blue;
	else
		color = sf::Color::Yellow;

	std::shared_ptr<Client> newClient = std::make_shared<Client>(ip, name, color, index, numPort);
	clients.push_back(newClient);
}

std::vector<std::shared_ptr<Client>> GameManager::RecognizeClient(int index)
{
	std::vector<std::shared_ptr<Client>> otherClients;

	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i]->GetIndex() == index)
			referenceClient = clients[i];
		else if (i > index)
			otherClients.push_back(clients[i]);
	}

	for (int i = 0; i < clients.size(); i++)
		map->SetName(i, clients[i]->GetName());

	return otherClients;
}
