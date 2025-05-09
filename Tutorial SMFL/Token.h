#pragma once
#include "Cell.h"
#include <memory>
#include <SFML/Graphics.hpp>

#define TOKEN_RADIUS 15

class Token
{
private:
	std::shared_ptr<Cell> currentCell;
	std::shared_ptr<Cell> originCell;
	bool isInGame;
	int id;
	const sf::Color color;
	sf::CircleShape shape;

public:
	Token(std::shared_ptr<Cell> _currentCell, const sf::Color _color, int id);

	void Render(sf::RenderWindow& window);
	int MoveToken(int moves);
	void ReturnToOriginalCell();

	inline std::shared_ptr<Cell> GetCurrentCell() const { return currentCell; }
	inline std::shared_ptr<Cell> GetOriginCell() const { return originCell; }
	inline bool GetIsInGame() const { return isInGame; }
	inline sf::CircleShape& GetShape() { return shape; }
	inline sf::Color GetColor() const { return color; }
	inline int GetId() const { return id; }

	inline void SetIsInGame(const bool state) { isInGame = state; }
	inline void SetCurrentCell(std::shared_ptr<Cell> _currentCell) { currentCell = _currentCell; }
};
