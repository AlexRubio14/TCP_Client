#pragma once
#include "Cell.h"

#define TOKEN_RADIUS 15

class Token
{
private:
	Cell* currentCell = new Cell();
	bool isInGame;
	sf::Color color;
	sf::CircleShape shape;

public:
	Token(Cell* _currentCell, sf::Color _color);

	void Render(sf::RenderWindow& window);
	void MoveToken(int moves);

	inline Cell* GetCurrentCell() { return currentCell; }
	inline bool GetIsInGame() { return isInGame; }
};

