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

public:
	Cell(int _id, Color _color);
	Cell();
	~Cell();

	void Render(sf::RenderWindow& window);

	inline int GetId() { return id; }
	inline Color GetColor() { return color; }
	inline std::vector<Cell*> GetNextCells() { return *nextCells; }
	inline sf::Vector2f& GetPosition() { return position; }

	inline void SetNextCells(Cell* nextCell) { nextCells->push_back(nextCell); }
};

