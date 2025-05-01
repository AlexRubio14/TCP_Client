#include "Player.h"
#include "GameManager.h"
#include <iostream>

Player::Player(std::string _name, sf::Color _color)
{
	name = _name;
	color = _color;
	extraMoves = false;

	int idToSpawn = SPAWN_BLUE;
	if (color == sf::Color::Yellow)
		idToSpawn += COLORED_CELLS_PER_COLOR;
	else if (color == sf::Color::Red)
		idToSpawn += (COLORED_CELLS_PER_COLOR * 2);
	else if (color == sf::Color::Green)
		idToSpawn += (COLORED_CELLS_PER_COLOR * 3);

	for (int i = 0; i < 4; i++)
	{
		Token* token = new Token(GAME.GetMap()->GetCells()[idToSpawn + i], color);
		tokens.push_back(token);
	}
}

void Player::ControlDice()
{
	diceValue = ThrowDice();
	if (AllTokensInBase() && diceValue != 5)
	{
		if (diceValue == 6)
		{
			canThrowDice = true;
			std::cout << "Player del color: " << GetColorString() << " Ha sacado un: " << diceValue << " Pero no puede sacar una casilla de su base pero vuelves a tirar" << std::endl;
			return;
		}
		GAME.EndTurn();
		std::cout << "Player del color: " << GetColorString() << " Ha sacado un: " << diceValue << " Pero no puede sacar una casilla de su base" << std::endl;
		return;
	}
	canThrowDice = false;
	if (!AnyTokenInBase() && diceValue == 6)
		diceValue = 7;
	std::cout << "Player del color: " << GetColorString() << " Ha sacado un: " << diceValue << std::endl;
}

int Player::ThrowDice()
{
	std::cout << std::endl;
	std::cout << "Introduce 0 para numero random: ";
	int hardcodedDice;
	std::cin >> hardcodedDice;
	std::cout << std::endl;

	if (hardcodedDice > 0)
		return hardcodedDice;

	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	return (std::rand() % 6) + 1;
}

void Player::SelectToken(sf::Vector2f mousePosition)
{
	for (Token* token : tokens)
	{
		sf::Vector2f circleCenter = token->GetShape().getPosition();
		float radius = token->GetShape().getRadius();

		float dx = mousePosition.x - circleCenter.x;
		float dy = mousePosition.y - circleCenter.y;
		float distanceSquared = dx * dx + dy * dy;

		if (distanceSquared <= radius * radius)
		{
			ControlInteraction(token);
			ControlNextTurn(token);		
			return;
		}
	}
}

void Player::ControlInteraction(Token* token)
{
	if (diceValue == 5)
	{
		if (AnyTokenInBase())
		{
			if (!token->GetIsInGame())
			{
				//Forzar sacar ficha si sale 5 y hay fichas en base
				std::cout << "Sacas la casilla de base" << std::endl;
				diceValue = token->MoveToken(1);
			}
			else
				return;

		}
		else
		{
			//Mover ficha
			std::cout << "Mueves el token: " << diceValue << " casillas" << std::endl;
			diceValue = token->MoveToken(diceValue);
		}
	}
	else
	{
		if (token->GetIsInGame())
		{
			//Mover ficha
			std::cout << "Mueves el token: " << diceValue << " casillas" << std::endl;
			diceValue = token->MoveToken(diceValue);
		}
		else
			return;
	}
}

void Player::ControlNextTurn(Token* token)
{
	if (diceValue == 6 || diceValue == 7)
	{
		canThrowDice = true;
		std::cout << "Vuelve a tirar" << std::endl;
		return;
	}
	else if ((diceValue == 10 || diceValue == 20) && extraMoves)
	{
		extraMoves = false;
		if (diceValue == 10)
		{
			auto it = std::find(tokens.begin(), tokens.end(), token);
			if (it != tokens.end() && token->GetCurrentCell()->GetNextCells().empty())
			{
				tokens.erase(it);
			}
		}

		if (!AllTokensInBase())
		{
			std::cout << "Muevete otra vez" << std::endl;
			return;
		}
		else
		{
			std::cout << "Pringado no sigues" << std::endl;
			return;
		}
	}

	GAME.EndTurn();
}

void Player::Update(sf::RenderWindow& window)
{
	for (Token* token : tokens)
	{
		token->Render(window);
	}
}

void Player::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		switch (mousePressed->button)
		{
		case sf::Mouse::Button::Left:
			if (canThrowDice)
				ControlDice();
			else
				SelectToken(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			break;
		}
	}
}

std::string Player::GetColorString()
{
	if (color == sf::Color::Green)
		return "Green";
	if (color == sf::Color::Blue)
		return "Blue";
	if (color == sf::Color::Red)
		return "Red";
	if (color == sf::Color::Yellow)
		return "Yellow";
	return " ";
}

bool Player::AllTokensInBase()
{
	for (Token* token : tokens)
		if (token->GetIsInGame())
			return false;

	return true;
}

bool Player::AnyTokenInBase()
{
	for (Token* token : tokens)
		if (!token->GetIsInGame())
			return true;

	return false;
}


