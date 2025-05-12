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

	/*if (TIME.IsTurnTimeOver())
	{
		std::cout << "Se acabó el tiempo, cambio de turno";
		EndTurn();
	}*/

	if (referenceClient->GetPlayerData().GetIndex() == currentClient->GetPlayerData().GetIndex() && !endGame)
	{
		currentClient->HandleEvent(event, window);
		HandleEvent(event, window);
	}

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
	{
		window.close();
		CustomPacket customPacket(DISCONNECT);
		EVENT_MANAGER.Emit(DISCONNECT, customPacket);
	}
}

void GameManager::StartTurn()
{
	currentClient->GetPlayerData().SetCanThrowDice(true);
	currentClient->GetPlayerData().ResetDiceValue();
	TIME.StartTurn();
}

void GameManager::EndTurn(bool reciveMessage)
{
	if (!reciveMessage)
		SendEndTurn();
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

void GameManager::SendEndTurn()
{
	std::cout << "End Turn" << std::endl;
	CustomPacket packet(END_TURN);
	for (int i = 0; i < NETWORK.GetClients().size(); i++)
	{
		if (GAME.GetReferenceClient()->GetPlayerData().GetIndex() == NETWORK.GetClients()[i]->GetPlayerData().GetIndex())
			continue;

		packet.packet << "End Turn";

		PACKET_MANAGER.SendPacketToClient(NETWORK.GetClients()[i], packet);
	}
}

void GameManager::ResetGame()
{
	clients.clear();
	currentClient = nullptr;
	endGame = false;
}

const std::shared_ptr<Token>& GameManager::TokenInPosition(Token* tokenChecked)
{
	for (const std::shared_ptr<Client>& client : clients)
	{
		for (const std::shared_ptr<Token>& token : client->GetPlayerData().GetTokens())
		{
			if (token->GetCurrentCell() == tokenChecked->GetCurrentCell() && token.get() != tokenChecked)
				return token;
		}
	}

	return nullptr;
}

void GameManager::AddClient(const std::string &ip, const std::string &name, const int &index, const int& numPort, const std::string& guid)
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

	NetworkClient networkClient(ip, numPort, guid);
	PlayerData playerData(name, color, index);

	std::shared_ptr<Client> newClient = std::make_shared<Client>(networkClient, playerData);
	clients.push_back(newClient);

	std::cout << "Adding client: IP = " << ip << ", Port = " << numPort << std::endl;
}

void GameManager::RecognizeClient(int index)
{
	referenceClient = clients[index];

	std::cout <<"NUM DE CLIENTES: " << clients.size() << std::endl;

	for (int i = 0; i < clients.size(); i++)
		map->SetName(i, clients[i]->GetPlayerData().GetUsername());
}
