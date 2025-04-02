#pragma once
#include "Cell.h"

class Token
{
private:
	Cell* currentCell = new Cell();
	bool isInGame;

public:
	Token();

	inline Cell* GetCurrentCell() { return currentCell; }
	inline bool GetIsInGame() { return isInGame; }
};

