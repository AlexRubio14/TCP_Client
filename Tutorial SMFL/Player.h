#pragma once
#include "Token.h"

class Player
{
private:
	std::vector<Token*> tokens;
	std::string name;
	sf::Color color;
	bool canThrowDice;
	int diceValue;

	bool extraMoves;

public:
	Player(std::string _name, sf::Color _color);
	int ThrowDice();
	void SelectToken(sf::Vector2f mousePosition);
	void Update(sf::RenderWindow& window);

	void HandleEvent(const sf::Event& event, sf::RenderWindow& window);
	std::string GetColorString();

	bool AllTokensInBase();
	bool AnyTokenInBase();

	inline std::vector<Token*> GetTokens() { return tokens; }
	inline std::string GetName() { return name; }
	
	inline void SetCanThrowDice(bool state) { canThrowDice = state; }
	inline void SetExtraMoves(bool state) { extraMoves = state; }
};

