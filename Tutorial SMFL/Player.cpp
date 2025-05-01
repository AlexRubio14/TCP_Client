#include "Player.h"
#include "GameManager.h"
#include <iostream>

Player::Player(std::string _name, sf::Color _color)
{
	name = _name;
	color = _color;

	int idToSpawn = 49;
	if (color == sf::Color::Yellow)
		idToSpawn += 9;
	else if (color == sf::Color::Red)
		idToSpawn += (9 * 2);
	else if (color == sf::Color::Green)
		idToSpawn += (9 * 3);

	for (int i = 0; i < 4; i++)
	{
		Token* token = new Token(GAME.GetMap()->GetCells()[idToSpawn + i], color);
		tokens.push_back(token);
	}
}

int Player::ThrowDice()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	return (std::rand() % 6) + 1;
}

void Player::SelectToken()
{
	//seleccionas una ficha
	//y haces el move
}

void Player::Update(sf::RenderWindow& window)
{
	std::cout << "NIGGER" << std::endl;
	std::cout << "GAY" << std::endl;
	for (Token* token : tokens)
	{
		token->Render(window);
	}
}


