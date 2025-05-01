#pragma once
#include "Map.h"
#include "Player.h"

#define GAME GameManager::Instance()

class GameManager
{
private:
	Map* map;
	std::vector<Player*> players;

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
	void Update(sf::RenderWindow& window);

	inline Map* GetMap() { return map; }
};

