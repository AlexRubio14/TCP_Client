#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Token.h"

class PlayerData
{
private:

	std::string username;
	sf::Color color;
	int index;

	std::vector<std::shared_ptr<Token>> tokens;

	bool extraMoves;
	bool canThrowDice;
	int diceValue;

public:

	PlayerData(const std::string& username, const sf::Color& color, int index);

	int ThrowDice();
	void ControlDice();
	void SelectToken(sf::Vector2f mousePosition);
	bool ControlInteraction(const std::shared_ptr<Token>& token);
	void ControlNextTurn(const std::shared_ptr<Token>& token);
	void SendDiceValue();
	void SendMoveToken(int value, int tokenID);
	inline void ResetDiceValue() { diceValue = 0; }

	void Update(sf::RenderWindow& window);
	void HandleEvent(const sf::Event& event, sf::RenderWindow& window);
	void EraseToken(int index);

	bool AllTokensInBase() const;
	bool AnyTokenInBase() const;
	bool AllTokensEndGame() const;
	bool HasTokenInSameCell();

	const std::string& GetUsername() const { return username; }
	const sf::Color& GetColor() const { return color; }
	int GetIndex() const { return index; }
	std::vector<std::shared_ptr<Token>> GetTokens() const { return tokens; }
	std::string GetColorString();

	inline void SetCanThrowDice(const bool value) { canThrowDice = value; }
	inline void SetExtraMoves(const bool value) { extraMoves = value; }



};

