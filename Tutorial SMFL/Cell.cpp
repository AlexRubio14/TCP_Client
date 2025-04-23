#include "Cell.h"

void Cell::CreateNormalCell()
{
	square.setSize({ 48.f, 48.f });

	switch (color)
	{
	case WHITE:
		square.setFillColor(sf::Color::Transparent);
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

void Cell::CreateSafeZone()
{
	safeZone = new sf::ConvexShape();
	safeZone->setPointCount(3);
	safeZone->setFillColor(square.getFillColor());
	safeZone->setOutlineThickness(2.f);
	safeZone->setOutlineColor(sf::Color::Black);
}

Cell::Cell(int _id, Color _color)
{
	id = _id;
	color = _color;
	nextCells = new std::vector<Cell*>();
	render = true;

	CreateNormalCell();

	if (nextCells->size() == 0)
		CreateSafeZone();
}

Cell::Cell()
{
}

Cell::~Cell()
{
	delete safeZone;
}

void Cell::Render(sf::RenderWindow& window)
{
	if (!render)
		return;

	if (nextCells->size() != 0)
		window.draw(square);
	else
		window.draw(*safeZone);
}

void Cell::SetPositionSquare(sf::Vector2f _position)
{
	position = _position;
	square.setPosition(position);
}

void Cell::SetPositionTriangles(sf::Vector2f _position, sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3, float angle)
{
	safeZone->setPoint(0, point1);
	safeZone->setPoint(1, point2);
	safeZone->setPoint(2, point3);
	safeZone->setRotation(sf::degrees(angle));

	position = _position;
	safeZone->setPosition(position);
}

