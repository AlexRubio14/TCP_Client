#pragma once
#include "Cell.h"
#include <memory>
#include <SFML/Graphics.hpp>

#define TOKEN_RADIUS 15

enum TokenState
{
	IN_BASE,
	IN_GAME,
	END_GAME
};

class Token
{
private:
	std::shared_ptr<Cell> currentCell;
	std::shared_ptr<Cell> originCell;
	TokenState tokenState;
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
	inline TokenState GetIsInGame() const { return tokenState; }
	inline sf::CircleShape& GetShape() { return shape; }
	inline sf::Color GetColor() const { return color; }
	inline int GetId() const { return id; }

	inline void ChangeTokenState(const TokenState state) { tokenState = state; }
	inline void SetCurrentCell(std::shared_ptr<Cell> _currentCell) { currentCell = _currentCell; }
};
