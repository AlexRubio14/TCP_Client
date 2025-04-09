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
	void SetCellsPosition(sf::RenderWindow& window);
	void SetWhiteCellsPosition(sf::Vector2f& currentPosition);
	void SetBlueCells(sf::RenderWindow& window, sf::Vector2f& currentPosition);
	void SetYellowCells(sf::RenderWindow& window, sf::Vector2f& currentPosition);
	void SetRedCells(sf::RenderWindow& window, sf::Vector2f& currentPosition);
	void SetGreenCells(sf::RenderWindow& window, sf::Vector2f& currentPosition);
	void SetHomePositions(sf::Vector2f& currentPosition, int initId);
public:
	Map(sf::RenderWindow& window);
	~Map();

	void Update(sf::RenderWindow& window);

	void PrintMap();
};

