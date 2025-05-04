#pragma once
#include "Map.h"
#include "Client.h"

#define GAME GameManager::Instance()

class GameManager
{
private:
	Map* map;
	std::vector<Client*> clients;

	Client* currentClient;
	Client* client;
	int currentClientIndex;

	GameManager() = default;

	GameManager(const GameManager&) = delete;
	GameManager& operator =(const GameManager&) = delete;

public:
	inline static GameManager& Instance()
	{
		static GameManager manager;
		return manager;
	}

	void Init(sf::RenderWindow& _window);
	void Update(sf::RenderWindow& window, const sf::Event& event);

	void HandleEvent(const sf::Event& event, sf::RenderWindow& window);
	void StartTurn();
	void EndTurn();
	void StartGame();

	Token* TokenInPosition(Token* tokenChecked);

	void AddClient(const std::string& ip, const std::string& name, const int& index);
	void RecognizeClient(int index);

	inline Map* GetMap() { return map; }
	inline Client* GetCurrentClient() { return currentClient; }
	inline std::vector<Client*> GetClients() { return clients; }
};

