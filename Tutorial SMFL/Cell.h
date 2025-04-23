#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

enum Color
{
	WHITE,
	YELLOW,
	BLUE,
	RED, 
	GREEN
};

class Cell
{
private:
	int id;
	Color color;
	std::vector<Cell*>* nextCells;
	sf::Vector2f position;
	sf::RectangleShape square;
	sf::ConvexShape* safeZone;

	bool render;
	void CreateNormalCell();
	void CreateSafeZone();

public:
	Cell(int _id, Color _color);
	Cell();
	~Cell();

	void Render(sf::RenderWindow& window);
	void SetPositionSquare(sf::Vector2f _position);
	void SetPositionTriangles(sf::Vector2f _position, sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3, float angle);



	inline int GetId() { return id; }
	inline Color GetColor() { return color; }
	inline std::vector<Cell*> GetNextCells() { return *nextCells; }
	inline sf::Vector2f& GetPosition() { return position; }
	inline sf::RectangleShape GetSquare() { return square; }

	inline void SetNextCells(Cell* nextCell) { nextCells->push_back(nextCell); }
	inline void SetRender(bool _render) { render = _render; }
};

