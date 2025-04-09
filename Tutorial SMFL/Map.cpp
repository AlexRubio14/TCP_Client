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

void Map::SetCellsPosition(sf::RenderWindow& window)
{
	sf::Vector2f currentPosition;
	currentPosition.x = window.getSize().x - 150.f;
	currentPosition.y = window.getSize().y / 2 + 25.f;
	
	SetWhiteCellsPosition(currentPosition);
	SetBlueCells(window, currentPosition);
	SetYellowCells(window, currentPosition);
	SetRedCells(window, currentPosition);
	SetGreenCells(window, currentPosition);
}

void Map::SetWhiteCellsPosition(sf::Vector2f& currentPosition)
{
	for (int i = 0; i < 5; i++)
	{
		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;

		currentPosition.x -= 50.f;
	}

	for (int i = 5; i < 10; i++)
	{
		currentPosition.y += 50.f;

		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;
	}

	currentPosition.x -= 50.f;
	cells[10]->GetPosition().x = currentPosition.x;
	cells[10]->GetPosition().y = currentPosition.y;
	currentPosition.x -= 50.f;

	for (int i = 11; i < 16; i++)
	{
		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;

		currentPosition.y -= 50.f;
	}

	for (int i = 16; i < 21; i++)
	{
		currentPosition.x -= 50.f;

		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;
	}

	currentPosition.y -= 50.f;
	cells[21]->GetPosition().x = currentPosition.x;
	cells[21]->GetPosition().y = currentPosition.y;
	currentPosition.y -= 50.f;

	for (int i = 22; i < 27; i++)
	{
		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;

		currentPosition.x += 50.f;
	}

	for (int i = 27; i < 32; i++)
	{
		currentPosition.y -= 50.f;

		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;
	}

	currentPosition.x += 50.f;
	cells[32]->GetPosition().x = currentPosition.x;
	cells[32]->GetPosition().y = currentPosition.y;
	currentPosition.x += 50.f;

	for (int i = 33; i < 38; i++)
	{
		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;

		currentPosition.y += 50.f;
	}

	for (int i = 38; i < 43; i++)
	{
		currentPosition.x += 50.f;

		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;
	}

	currentPosition.y += 50.f;
	cells[43]->GetPosition().x = currentPosition.x;
	cells[43]->GetPosition().y = currentPosition.y;
}

void Map::SetBlueCells(sf::RenderWindow& window, sf::Vector2f& currentPosition)
{
	for (int i = 44; i < 49; i++)
	{
		currentPosition.x -= 50.f;
		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;
	}

	currentPosition.x = window.getSize().x * 3 / 4;
	currentPosition.y = window.getSize().y * 3 / 4;

	SetHomePositions(currentPosition, 49);
}

void Map::SetYellowCells(sf::RenderWindow& window, sf::Vector2f& currentPosition)
{
	currentPosition = cells[10]->GetPosition();
	for (int i = 53; i < 58; i++)
	{
		currentPosition.y -= 50.f;
		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;
	}

	currentPosition.x = window.getSize().x / 4;
	currentPosition.y = window.getSize().y * 3 / 4;

	SetHomePositions(currentPosition, 58);
}

void Map::SetRedCells(sf::RenderWindow& window, sf::Vector2f& currentPosition)
{
	currentPosition = cells[21]->GetPosition();
	for (int i = 62; i < 67; i++)
	{
		currentPosition.x += 50.f;
		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;
	}

	currentPosition.x = window.getSize().x / 4;
	currentPosition.y = window.getSize().y / 4;

	SetHomePositions(currentPosition, 67);
}

void Map::SetGreenCells(sf::RenderWindow& window, sf::Vector2f& currentPosition)
{
	currentPosition = cells[32]->GetPosition();
	for (int i = 71; i < 76; i++)
	{
		currentPosition.y += 50.f;
		cells[i]->GetPosition().x = currentPosition.x;
		cells[i]->GetPosition().y = currentPosition.y;
	}

	currentPosition.x = window.getSize().x * 3 / 4;
	currentPosition.y = window.getSize().y / 4;

	SetHomePositions(currentPosition, 76);
}

void Map::SetHomePositions(sf::Vector2f& currentPosition, int initId)
{
	currentPosition.x -= 25.f;
	currentPosition.y -= 75.f;
	cells[initId]->GetPosition().x = currentPosition.x;
	cells[initId]->GetPosition().y = currentPosition.y;
	initId++;

	currentPosition.x += 50.f;
	currentPosition.y += 50.f;
	cells[initId]->GetPosition().x = currentPosition.x;
	cells[initId]->GetPosition().y = currentPosition.y;
	initId++;

	currentPosition.x -= 100.f;
	cells[initId]->GetPosition().x = currentPosition.x;
	cells[initId]->GetPosition().y = currentPosition.y;
	initId++;

	currentPosition.x += 50.f;
	currentPosition.y += 50.f;
	cells[initId]->GetPosition().x = currentPosition.x;
	cells[initId]->GetPosition().y = currentPosition.y;
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

Map::Map(sf::RenderWindow& window)
{
	CreateCells();
	SetNextCells();
	SetCellsPosition(window);
}

Map::~Map()
{
	
}

void Map::Update(sf::RenderWindow& window)
{
	window.clear(sf::Color(238, 208, 157));



	for (int i = 0; i < cells.size(); i++)
		cells[i]->Render(window);

	window.display();
}
