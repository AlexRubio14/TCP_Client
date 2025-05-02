#pragma once
#include "Cell.h"


#define TOKEN_RADIUS 15

class Token
{
private:
	Cell* currentCell = new Cell();
	Cell* originCell = new Cell();
	bool isInGame;
	sf::Color color;
	sf::CircleShape shape;

public:
	Token(Cell* _currentCell, sf::Color _color);

	void Render(sf::RenderWindow& window);
	int MoveToken(int moves);
	void ReturnToOriginalCell();

	inline Cell* GetCurrentCell() { return currentCell; }
	inline Cell* GetOriginCell() { return originCell; }
	inline bool GetIsInGame() { return isInGame; }
	inline sf::CircleShape GetShape() { return shape; }
	inline sf::Color GetColor() { return color; }
	inline void SetIsInGame(bool state) { isInGame = state; }
	inline void SetCurrentCell(Cell* _currentCell) { currentCell = currentCell; }

};

