#pragma once
#include "Map.h"
#include "Player.h"

#define GAME GameManager::Instance()

class GameManager
{
private:
	Map* map;
	std::vector<Player*> players;

	Player* currentPlayer;
	int currentPlayerIndex;

	GameManager() = default;

	GameManager(const GameManager&) = delete;
	GameManager& operator =(const GameManager&) = delete;

public:
	inline static GameManager& Instance()
	{
		static GameManager manager;
		return manager;
	}

	void Init(Map* _map);
	void Update(sf::RenderWindow& window, const sf::Event& event);

	void HandleEvent(const sf::Event& event, sf::RenderWindow& window);
	void StartTurn();
	void EndTurn();

	Token* TokenInPosition(Cell* currentCell);

	inline Map* GetMap() { return map; }
	inline Player* GetCurrentPlayer() { return currentPlayer; }
};

