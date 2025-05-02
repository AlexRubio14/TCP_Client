#include "Map.h"
#include <iostream>
#include "GameManager.h"
#include "SceneManager.h"

#pragma region CreateMap
sf::Color Map::SetCellColor(int _id)
{
	if (_id <= WHITE_CELLS)
		return sf::Color::Transparent;
	if (_id <= WHITE_CELLS + COLORED_CELLS_PER_COLOR)
		return sf::Color::Blue;
	if (_id <= WHITE_CELLS + (COLORED_CELLS_PER_COLOR * 2))
		return sf::Color::Yellow;
	if (_id <= WHITE_CELLS + (COLORED_CELLS_PER_COLOR * 3))
		return sf::Color::Red;
	return sf::Color::Green;
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

void Map::CreateBases()
{
	bases.push_back(sf::CircleShape(100.f));
	bases.push_back(sf::CircleShape(100.f));
	bases.push_back(sf::CircleShape(100.f));
	bases.push_back(sf::CircleShape(100.f));

	for (int i = 0; i < bases.size(); i++)
	{
		bases[i].setOutlineThickness(2.f);
		bases[i].setOutlineColor(sf::Color::Black);
	}
}

void Map::SetPlayersName()
{
	font = SCENE.GetCurrentScene()->GetFont();
	sf::RectangleShape square = squares[0];

	for (int i = 0; i< 4; i++)
	{
		names.push_back(new sf::Text(font));
		names[i]->setFillColor(sf::Color::Black);
		names[i]->setString(" ");
	}

	names[0]->setPosition(sf::Vector2f(
		square.getPosition().x - names[0]->getLocalBounds().position.x,
		(square.getPosition().y/1.15f) - names[0]->getLocalBounds().position.y));

	names[1]->setPosition(sf::Vector2f(
		square.getPosition().x + square.getSize().x - square.getSize().x/2.65 - names[1]->getLocalBounds().position.x,
		(square.getPosition().y / 1.15f) - names[1]->getLocalBounds().position.y));
	
	names[2]->setPosition(sf::Vector2f(
		square.getPosition().x + square.getSize().x - square.getSize().x / 2.65 - names[2]->getLocalBounds().position.x,
		square.getPosition().y + square.getSize().y));

	names[3]->setPosition(sf::Vector2f(
		square.getPosition().x - names[3]->getLocalBounds().position.x,
		square.getPosition().y + square.getSize().y));
}

void Map::CreateBoard(sf::RenderWindow& window)
{
	sf::Color brown(238, 208, 157);

	squares.push_back(sf::RectangleShape({ 646, 646 }));
	squares.push_back(sf::RectangleShape({ (float)window.getSize().x - 20, (float)window.getSize().y - 20 }));

	squares[0].setFillColor(sf::Color::Transparent);
	squares[0].setPosition(sf::Vector2f(100, 100));
	squares[0].setOutlineThickness(4.f);
	squares[0].setOutlineColor(sf::Color::Black);

	squares[1].setFillColor(brown);
	squares[1].setPosition(sf::Vector2f(10, 10));
	squares[1].setOutlineThickness(10.f);
	squares[1].setOutlineColor(sf::Color::Black);

	SetPlayersName();
}

void Map::SetCellsPosition(sf::RenderWindow& window)
{
	sf::Vector2f currentPosition = sf::Vector2f(window.getSize().x - 150.f, window.getSize().y / 2 + 25.f);

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
		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));

		currentPosition.x -= 50.f;
	}

	for (int i = 5; i < 10; i++)
	{
		currentPosition.y += 50.f;

		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	}

	currentPosition.x -= 50.f;
	cells[10]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	currentPosition.x -= 50.f;

	for (int i = 11; i < 16; i++)
	{
		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));

		currentPosition.y -= 50.f;
	}

	for (int i = 16; i < 21; i++)
	{
		currentPosition.x -= 50.f;

		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	}

	currentPosition.y -= 50.f;
	cells[21]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	currentPosition.y -= 50.f;

	for (int i = 22; i < 27; i++)
	{
		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));

		currentPosition.x += 50.f;
	}

	for (int i = 27; i < 32; i++)
	{
		currentPosition.y -= 50.f;

		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	}

	currentPosition.x += 50.f;
	cells[32]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	currentPosition.x += 50.f;

	for (int i = 33; i < 38; i++)
	{
		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));

		currentPosition.y += 50.f;
	}

	for (int i = 38; i < 43; i++)
	{
		currentPosition.x += 50.f;

		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	}

	currentPosition.y += 50.f;
	cells[43]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
}

void Map::SetBlueCells(sf::RenderWindow& window, sf::Vector2f& currentPosition)
{
	for (int i = 44; i < 48; i++)
	{
		currentPosition.x -= 50.f;
		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	}
	currentPosition.x -= 50.f;
	cells[48]->SetPositionTriangles(sf::Vector2f(currentPosition.x, currentPosition.y),
		sf::Vector2f(-25.f, -25.f), sf::Vector2f(-98.f, 48.f), sf::Vector2f(48.f, 48.f), -90);

	currentPosition.x = window.getSize().x * 3 / 4;
	currentPosition.y = window.getSize().y * 3 / 4;

	SetHomePositions(currentPosition, 49, 0);
}

void Map::SetYellowCells(sf::RenderWindow& window, sf::Vector2f& currentPosition)
{
	currentPosition = cells[10]->GetPosition();
	for (int i = 53; i < 57; i++)
	{
		currentPosition.y -= 50.f;
		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	}
	currentPosition.y -= 50.f;
	cells[57]->SetPositionTriangles(sf::Vector2f(currentPosition.x, currentPosition.y),
		sf::Vector2f(25.f, -25.f), sf::Vector2f(98.f, 48.f), sf::Vector2f(-48.f, 48.f), 0);

	currentPosition.x = window.getSize().x / 4;
	currentPosition.y = window.getSize().y * 3 / 4;

	SetHomePositions(currentPosition, 58, 1);
}

void Map::SetRedCells(sf::RenderWindow& window, sf::Vector2f& currentPosition)
{
	currentPosition = cells[21]->GetPosition();
	for (int i = 62; i < 66; i++)
	{
		currentPosition.x += 50.f;
		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	}
	currentPosition.x += 50.f;
	cells[66]->SetPositionTriangles(sf::Vector2f(currentPosition.x, currentPosition.y),
		sf::Vector2f(25.f, -75.f), sf::Vector2f(-48.f, 0.f), sf::Vector2f(98.f, 0.f), 90);

	currentPosition.x = window.getSize().x / 4;
	currentPosition.y = window.getSize().y / 4;

	SetHomePositions(currentPosition, 67, 2);
}

void Map::SetGreenCells(sf::RenderWindow& window, sf::Vector2f& currentPosition)
{
	currentPosition = cells[32]->GetPosition();
	for (int i = 71; i < 75; i++)
	{
		currentPosition.y += 50.f;
		cells[i]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	}
	currentPosition.y += 50.f;
	cells[75]->SetPositionTriangles(sf::Vector2f(currentPosition.x, currentPosition.y),
		sf::Vector2f(-25.f, -75.f), sf::Vector2f(48.f, 0.f), sf::Vector2f(-98.f, 0.f), 180);

	currentPosition.x = window.getSize().x * 3 / 4;
	currentPosition.y = window.getSize().y / 4;

	SetHomePositions(currentPosition, 76, 3);
}

void Map::SetHomePositions(sf::Vector2f& currentPosition, int initId, int baseId)
{
	sf::Color baseColor = cells[initId]->GetSquare().getFillColor();
	bases[baseId].setFillColor(baseColor);
	bases[baseId].setPosition(sf::Vector2f(currentPosition.x - bases[baseId].getRadius(), currentPosition.y - bases[baseId].getRadius()));

	currentPosition.x -= 25.f;
	currentPosition.y -= 75.f;
	cells[initId]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	cells[initId]->SetRender(false);
	initId++;

	currentPosition.x += 50.f;
	currentPosition.y += 50.f;
	cells[initId]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	cells[initId]->SetRender(false);
	initId++;

	currentPosition.x -= 100.f;
	cells[initId]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	cells[initId]->SetRender(false);
	initId++;

	currentPosition.x += 50.f;
	currentPosition.y += 50.f;
	cells[initId]->SetPositionSquare(sf::Vector2f(currentPosition.x, currentPosition.y));
	cells[initId]->SetRender(false);
}

#pragma endregion

Map::Map(sf::RenderWindow& window)
{
	CreateBoard(window);
	CreateBases();
	CreateCells();
	SetNextCells();
	SetCellsPosition(window);
	SetPlayersName();
}

Map::~Map()
{

}

void Map::Update(sf::RenderWindow& window)
{
	Render(window);

	for (int i = 0; i < cells.size(); i++)
		cells[i]->Render(window);
}

void Map::Render(sf::RenderWindow& window)
{
	for (int i = 1; i >= 0; i--)
		window.draw(squares[i]);

	for (int i = 0; i < bases.size(); i++)
		window.draw(bases[i]);

	for (int i = 0; i < names.size(); i++)
		window.draw(*names[i]);
}
