#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Cell
{
private:
	int id;
	sf::Color color;
	std::vector<Cell*>* nextCells;
	sf::Vector2f position;
	sf::RectangleShape square;
	sf::ConvexShape* safeZone;
	int tokensInCell;

	bool render;
	void CreateNormalCell();
	void CreateSafeZone();

public:
	Cell(int _id, sf::Color _color);
	Cell();
	~Cell();

	void Render(sf::RenderWindow& window);
	void SetPositionSquare(sf::Vector2f _position);
	void SetPositionTriangles(sf::Vector2f _position, sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3, float angle);
	sf::Vector2f& SetTokenInCell();

	inline int GetId() { return id; }
	inline int GetTokensInCell() { return tokensInCell; }
	inline sf::Color GetColor() { return color; }
	inline std::vector<Cell*> GetNextCells() { return *nextCells; }
	inline sf::Vector2f& GetPosition() { return position; }
	inline sf::RectangleShape GetSquare() { return square; }

	inline void SetNextCells(Cell* nextCell) { nextCells->push_back(nextCell); }
	inline void SetRender(bool _render) { render = _render; }
	inline void SetTokensInCell(int cuantity) { tokensInCell += cuantity; }
};

