#pragma once
#include "Token.h"

class Player
{
private:
	std::vector<Token*> tokens;
	std::string name;
	sf::Color color;

public:
	Player(std::string _name, sf::Color _color);
	int ThrowDice();
	void SelectToken();
	void Update(sf::RenderWindow& window);

	inline std::vector<Token*> GetTokens() { return tokens; }
	inline std::string GetName() { return name; }
};

