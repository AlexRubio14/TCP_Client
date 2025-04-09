#include "Cell.h"

Cell::Cell(int _id, Color _color)
{
	id = _id;
	color = _color;
	nextCells = new std::vector<Cell*>();

	square.setSize({ 48.f, 48.f });

	switch (color)
	{
	case WHITE:
		square.setFillColor(sf::Color::White);
		break;
	case YELLOW:
		square.setFillColor(sf::Color::Yellow);
		break;
	case BLUE:
		square.setFillColor(sf::Color::Blue);
		break;
	case RED:
		square.setFillColor(sf::Color::Red);
		break;
	case GREEN:
		square.setFillColor(sf::Color::Green);
		break;
	default:
		break;
	}

	square.setOutlineThickness(2.f);
	square.setOutlineColor(sf::Color::Black);

}

Cell::Cell()
{
}

Cell::~Cell()
{
}

void Cell::Render(sf::RenderWindow& window)
{
	square.setPosition(position);
	window.draw(square);
}

