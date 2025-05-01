#include "Token.h"
#include <iostream>
#include "GameManager.h"

Token::Token(Cell* _currentCell, sf::Color _color)
{
	currentCell = _currentCell;
	isInGame = false;
	color = _color;
	shape = sf::CircleShape(TOKEN_RADIUS);
	shape.setFillColor(_color);
	shape.setPosition(currentCell->SetTokenInCell());
	shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color::Black);
	currentCell->SetTokensInCell(1);
}

void Token::Render(sf::RenderWindow& window)
{
	window.draw(shape);
}

int Token::MoveToken(int moves, bool leaveBase)
{
	int newDiceValue = moves;

	for (int i = 0; i < moves; i++)
	{
		std::vector<Cell*> nextCells = currentCell->GetNextCells();

		if (nextCells.empty())
		{
			std::cout << "La ficha ha llegado al final" << std::endl;
			newDiceValue = 10;
			break;
		}

		for (Cell* cells : nextCells)
			if (cells->GetTokensInCell() == 2)
			{
				std::cout << "Hay una barrera delante" << std::endl;
				return 0;
			}

		currentCell->SetTokensInCell(-1);

		if(nextCells.size() == 2 && nextCells[1]->GetColor() == color)
			currentCell = currentCell->GetNextCells()[1];
		else
			currentCell = currentCell->GetNextCells()[0];

		currentCell->SetTokensInCell(1);
	}
	if (leaveBase)
		isInGame = true;
	shape.setPosition(currentCell->SetTokenInCell());

	if (currentCell->GetTokensInCell() > 1)
	{
		Token* token = GAME.TokenInPosition(currentCell);
		if (token != nullptr)
			return ReturnToBase(token);
	}

	return newDiceValue;
}

int Token::ReturnToBase(Token* _token)
{
	_token->SetIsInGame(false);

	int idToSpawn = SPAWN_BLUE;
	if (_token->GetColor() == sf::Color::Yellow)
		idToSpawn += COLORED_CELLS_PER_COLOR;
	else if (_token->GetColor() == sf::Color::Red)
		idToSpawn += (COLORED_CELLS_PER_COLOR * 2);
	else if (_token->GetColor() == sf::Color::Green)
		idToSpawn += (COLORED_CELLS_PER_COLOR * 3);

	for (int i = 0; i < 4; i++)
	{
		if (GAME.GetMap()->GetCells()[idToSpawn - i]->GetTokensInCell() == 0)
		{
			_token->SetCurrentCell(GAME.GetMap()->GetCells()[idToSpawn + i]);
			_token->GetShape().setPosition(_token->GetCurrentCell()->SetTokenInCell());
			break;
		}
	}

	return 20;
}
