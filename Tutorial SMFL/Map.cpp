#include "Map.h"
#include <iostream>

Color Map::SetCellColor(int _id)
{
	if (_id <= WHITE_CELLS)
		return WHITE;
	if (_id <= WHITE_CELLS + COLORED_CELLS_PER_COLOR)
		return BLUE;
	if (_id <= WHITE_CELLS + (COLORED_CELLS_PER_COLOR * 2))
		return RED;
	if (_id <= WHITE_CELLS + (COLORED_CELLS_PER_COLOR * 3))
		return GREEN;
	return YELLOW;
}

void Map::CreateCells()
{
	for (int i = 1; i <= NUM_CELLS; i++)
	{
		Cell* newCell = new Cell(i, SetCellColor(i));
		cells.push_back(newCell);
	}
}

void Map::SetNextCells()
{
	//Setear el recorrido normal
	for (int i = 0; i < WHITE_CELLS - 1; i++)
	{
		cells[i]->SetNextCells(cells[i + 1]);
	}

	//Setear la ultima con la primera
	cells[WHITE_CELLS - 1]->SetNextCells(cells[0]);

	int j = 0;
	//setear los colores entre ellos
	for (int i = WHITE_CELLS; i < NUM_CELLS; i++)
	{
		j++;
		if (j >= 7)
		{
			j = 0;
			continue;
		}
		cells[i]->SetNextCells(cells[i + 1]);
	}

	cells[16]->SetNextCells(cells[68]);
	cells[33]->SetNextCells(cells[75]);
	cells[50]->SetNextCells(cells[82]);
	cells[67]->SetNextCells(cells[89]);
}

void Map::PrintMap()
{
	for (int i = 0; i < cells.size(); i++)
	{
		std::cout << "ID: " << cells[i]->GetId() << "   Color: " << cells[i]->GetColor() << "   Next: " << cells[i]->GetNextCells()->size() << std::endl;
	}
}

Map::Map()
{
	CreateCells();
	SetNextCells();
}

Map::~Map()
{
}
