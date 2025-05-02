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
	void ControlDice();
	void SelectToken(sf::Vector2f mousePosition);
	bool ControlInteraction(Token* token);
	void ControlNextTurn(Token* token);
	void Update(sf::RenderWindow& window);

	void HandleEvent(const sf::Event& event, sf::RenderWindow& window);
	std::string GetColorString();

	bool AllTokensInBase();
	bool AnyTokenInBase();
	bool HasTokensInSameCell() const;

	inline std::vector<Token*> GetTokens() const { return tokens; }
	inline std::string GetName() const { return name; }
	
	inline void SetCanThrowDice(const bool state) { canThrowDice = state; }
	inline void SetExtraMoves(const bool state) { extraMoves = state; }
};

