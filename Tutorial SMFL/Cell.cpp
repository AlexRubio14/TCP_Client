#include "Cell.h"

Cell::Cell(int _id, Color _color)
{
	id = _id;
	color = _color;
	nextCells = new std::vector<Cell*>();
}

Cell::~Cell()
{
}
