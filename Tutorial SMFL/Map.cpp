#include "Map.h"
#include <iostream>

Color Map::SetCellColor(int _id)
{
	if (_id <= WHITE_CELLS)
		return WHITE;
	if (_id <= WHITE_CELLS + COLORED_CELLS_PER_COLOR)
		return BLUE;
	if (_id <= WHITE_CELLS + (COLORED_CELLS_PER_COLOR * 2))
		return YELLOW;
	if (_id <= WHITE_CELLS + (COLORED_CELLS_PER_COLOR * 3))
		return RED;
	return GREEN;
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
	int spawnSeparation = 0;
	//setear los colores entre ellos
	for (int i = WHITE_CELLS; i < NUM_CELLS; i++)
	{
		j++;
		if (j >= 5)
		{
			//Setear la conexion del spawn
			for (int k = 0; k < SAVE_ZONE; k++)
			{
				i++;
				cells[i]->SetNextCells(cells[spawnSeparation]);
			}
			j = 0;
			spawnSeparation += SEPARATION_BETWEEN_SAVE_ZONE + 1;
			continue;
		}
		cells[i]->SetNextCells(cells[i + 1]);
	}

	//Setear las celdas con 2 celdas
	cells[43]->SetNextCells(cells[44]);
	cells[10]->SetNextCells(cells[53]);
	cells[21]->SetNextCells(cells[62]);
	cells[32]->SetNextCells(cells[71]);
}

void Map::PrintMap()
{
	for (int i = 0; i < cells.size(); i++)
	{
		if (cells[i]->GetNextCells().size() > 1)
			std::cout << "ID: " << cells[i]->GetId() << "   Color: " << cells[i]->GetColor() << "   Next: " << cells[i]->GetNextCells()[0]->GetId() << " , " << cells[i]->GetNextCells()[1]->GetId() << std::endl;
		else if(cells[i]->GetNextCells().size() > 0)
			std::cout << "ID: " << cells[i]->GetId() << "   Color: " << cells[i]->GetColor() << "   Next: " << cells[i]->GetNextCells()[0]->GetId() << std::endl;
		else
			std::cout << "ID: " << cells[i]->GetId() << "   Color: " << cells[i]->GetColor() << "   Next: " << 0 << std::endl;
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
