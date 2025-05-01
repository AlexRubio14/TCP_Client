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
	int MoveToken(int moves, bool leaveBase = false);
	int ReturnToBase(Token* _token);

	inline Cell* GetCurrentCell() { return currentCell; }
	inline bool GetIsInGame() { return isInGame; }
	inline sf::CircleShape GetShape() { return shape; }
	inline sf::Color GetColor() { return color; }
	inline void SetIsInGame(bool state) { isInGame = state; }
	inline void SetCurrentCell(Cell* _currentCell) { currentCell = currentCell; }

};

