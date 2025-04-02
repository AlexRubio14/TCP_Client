#pragma once
#include "Cell.h"

#define NUM_CELLS 80
#define WHITE_CELLS 44
#define COLORED_CELLS_PER_COLOR 9
#define SAVE_ZONE 4
#define SEPARATION_BETWEEN_SAVE_ZONE 10

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

