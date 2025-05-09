#include "Token.h"
#include <iostream>
#include "GameManager.h"

Token::Token(std::shared_ptr<Cell> _currentCell, sf::Color _color, int id)
	: currentCell(_currentCell), color(_color), id(id)
{
	originCell = currentCell;
	isInGame = false;
	shape = sf::CircleShape(TOKEN_RADIUS);
	shape.setFillColor(_color);
	shape.setPosition(currentCell->SetTokenInCell());
	shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color::Black);
	currentCell->AddTokensInCell(1);
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
	bool hasFinished = false;

	for (int i = 0; i < moves; i++)
	{
		std::vector<std::shared_ptr<Cell>> nextCells = currentCell->GetNextCells();

		if (nextCells[0]->GetNextCells().empty())
			hasFinished = true;

		std::shared_ptr<Cell> cellToGo = nullptr;

		if (nextCells.size() == 2 && nextCells[1]->GetColor() == color)
			cellToGo = currentCell->GetNextCells()[1];
		else
			cellToGo = currentCell->GetNextCells()[0];
		
		if (cellToGo->GetTokensInCell() == 2)
		{
			std::cout << "Hay una barrera delante" << std::endl;
			newDiceValue = 0;
			break;
		}
		
		currentCell->AddTokensInCell(-1);
		currentCell = cellToGo;
		currentCell->AddTokensInCell(1);

		if (hasFinished)
			break;
	}

	shape.setPosition(currentCell->SetTokenInCell());

	if (currentCell->GetTokensInCell() > 1)
	{
		std::shared_ptr<Token> token = GAME.TokenInPosition(this);
		if (token->GetColor() != color)
		{
			token->ReturnToOriginalCell();
			GAME.GetCurrentClient()->GetPlayerData().SetExtraMoves(true);
			return 20;
		}
		else if (token != nullptr)
		{
			shape.setPosition(currentCell->SetTokenInCell(1));
			token->GetShape().setPosition(currentCell->SetTokenInCell(3));
		}
	}

	if (hasFinished)
	{
		std::cout << "La ficha ha llegado al final" << std::endl;
		newDiceValue = 10;
		GAME.GetCurrentClient()->GetPlayerData().SetExtraMoves(true);
	}

	return newDiceValue;
}

void Token::ReturnToOriginalCell()
{
	isInGame = false;
	currentCell->AddTokensInCell(-1);
	currentCell = originCell;
	shape.setPosition(currentCell->SetTokenInCell());
}
