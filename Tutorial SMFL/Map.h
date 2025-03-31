#pragma once
#include "Cell.h"

#define NUM_CELLS 96
#define WHITE_CELLS 68
#define COLORED_CELLS_PER_COLOR 7

class Map
{
private:
	std::vector<Cell*> cells;
	
	Color SetCellColor(int _id);
	void CreateCells();
	void SetNextCells();

public:
	Map();
	~Map();

	void PrintMap();
};

