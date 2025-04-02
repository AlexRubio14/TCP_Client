#pragma once
#include <vector>

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

public:
	Cell(int _id, Color _color);
	Cell();
	~Cell();

	inline int GetId() { return id; }
	inline Color GetColor() { return color; }
	inline std::vector<Cell*> GetNextCells() { return *nextCells; }

	inline void SetNextCells(Cell* nextCell) { nextCells->push_back(nextCell); }
};

