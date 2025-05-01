#include "Token.h"

Token::Token(Cell* _currentCell, sf::Color _color)
{
	currentCell = _currentCell;
	isInGame = false;
	color = _color;
	shape = sf::CircleShape(TOKEN_RADIUS);
	shape.setFillColor(_color);
	shape.setPosition(currentCell->GetPosition());
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);
}

void Token::Render(sf::RenderWindow& window)
{
	window.draw(shape);
}

void Token::MoveToken(int moves)
{
	for (int i = 0; i < moves; i++)
	{
		//si hay dos nextCells comprobamos colores
		//desde GAME mirar si se puede acceder a la siguiente cell
		//si es que si avanzar
		//si es que no salir de la funcion
	}

	//comprobar si matamos ficha
	//render
}
