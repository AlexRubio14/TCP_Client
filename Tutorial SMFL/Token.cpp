#include "Token.h"
#include <iostream>
#include "GameManager.h"

Token::Token(Cell* _currentCell, sf::Color _color)
{
	currentCell = _currentCell;
	originCell = currentCell;
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

int Token::MoveToken(int moves)
{
	if (currentCell == originCell)
	{
		moves = 1;
		isInGame = true;
	}

	int newDiceValue = moves;

	for (int i = 0; i < moves; i++)
	{
		std::vector<Cell*> nextCells = currentCell->GetNextCells();

		if (nextCells.empty())
		{
			std::cout << "La ficha ha llegado al final" << std::endl;
			newDiceValue = 10;
			GAME.GetCurrentPlayer()->SetExtraMoves(true);
			break;
		}

		for (Cell* cells : nextCells)
		{
			if (cells->GetTokensInCell() == 2)
			{
				std::cout << "Hay una barrera delante" << std::endl;
				newDiceValue = 0;
				break;
			}
		}
			
		currentCell->SetTokensInCell(-1);

		if (nextCells.size() == 2 && nextCells[1]->GetColor() == color)
			currentCell = currentCell->GetNextCells()[1];
		else
			currentCell = currentCell->GetNextCells()[0];

		currentCell->SetTokensInCell(1);
	}

	shape.setPosition(currentCell->SetTokenInCell());

	if (currentCell->GetTokensInCell() > 1)
	{
		Token* token = GAME.TokenInPosition(currentCell);
		if (token != nullptr)
		{
			token->ReturnToOriginalCell();
			GAME.GetCurrentPlayer()->SetExtraMoves(true);
			return 20;
		}
	}

	return newDiceValue;
}

void Token::ReturnToOriginalCell()
{
	isInGame = false;
	currentCell = originCell;
	shape.setPosition(currentCell->SetTokenInCell());
}
