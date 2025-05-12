#pragma once
#include "Map.h"
#include "Client.h"

#define GAME GameManager::Instance()

class GameManager
{
private:
	std::unique_ptr<Map> map;
	std::vector<std::shared_ptr<Client>> clients;

	std::shared_ptr<Client> currentClient;
	std::shared_ptr<Client> referenceClient;
	int currentClientIndex;
	bool endGame;

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
	void EndTurn(bool reciveMessage = false);
	void StartGame();
	void SendEndTurn();
	void ResetGame();

	void ErasePlayer(int index);

	const std::shared_ptr<Token>& TokenInPosition(Token* tokenChecked);

	void AddClient(const std::string& ip, const std::string& name, const int& index, const int& numPort, const std::string& guid);
	void RecognizeClient(int index);

	inline const std::unique_ptr<Map>& GetMap() { return map; }
	inline const std::shared_ptr<Client>& GetCurrentClient() { return currentClient; }
	inline const std::vector<std::shared_ptr<Client>>& GetClients() { return clients; }
	inline const std::shared_ptr<Client>& GetReferenceClient() { return referenceClient; }
	
	inline void SetEndGame(bool state) { endGame = state; }

};

