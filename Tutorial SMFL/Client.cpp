#include "Client.h"

#include "PacketManager.h"
#include "EventManager.h"
#include "PacketType.h"
#include "SceneManager.h"
#include "GameManager.h"
#include <iostream>

Client::Client(const std::string& _ip, const std::string& _name, const sf::Color& _color, const int& _index)
	: ip(_ip), name(_name), color(_color), index(_index)
{
    socket = std::make_unique<sf::TcpSocket>();
	extraMoves = false;
	canThrowDice = false;
	diceValue = 0;

	int idToSpawn = SPAWN_BLUE;
	if (color == sf::Color::Yellow)
		idToSpawn += COLORED_CELLS_PER_COLOR;
	else if (color == sf::Color::Red)
		idToSpawn += (COLORED_CELLS_PER_COLOR * 2);
	else if (color == sf::Color::Green)
		idToSpawn += (COLORED_CELLS_PER_COLOR * 3);

	for (int i = 0; i < 4; i++)
	{
		std::shared_ptr<Token> token = std::make_shared<Token>(GAME.GetMap()->GetCells()[idToSpawn + i], color);
		tokens.push_back(token);
	}
}

void Client::HandleIncomingPackets()
{
    CustomPacket customPacket;

    sf::Socket::Status status = socket->receive(customPacket.packet);

    if (status == sf::Socket::Status::Done) {
        PACKET_MANAGER.ProcessPacket("", customPacket);
    }
    else if (status == sf::Socket::Status::Disconnected)
    {
        EVENT_MANAGER.Emit(DISCONNECT, "", customPacket);
    }
}

void Client::ControlDice()
{
	if (AllTokensInBase() && diceValue != 5)
	{
		if (diceValue == 6)
		{
			canThrowDice = true;
			std::cout << "Client del color: " << GetColorString() << " Ha sacado un: " << diceValue << " Pero no puede sacar una casilla de su base pero vuelves a tirar" << std::endl;
			diceValue = 0;
			return;
		}
		GAME.EndTurn();
		std::cout << "Client del color: " << GetColorString() << " Ha sacado un: " << diceValue << " Pero no puede sacar una casilla de su base" << std::endl;
		return;
	}
	canThrowDice = false;
	if (!AnyTokenInBase() && diceValue == 6)
		diceValue = 7;
	std::cout << "Client del color: " << GetColorString() << " Ha sacado un: " << diceValue << std::endl;
}

int Client::ThrowDice()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	return (std::rand() % 6) + 1;
}

void Client::SelectToken(sf::Vector2f mousePosition)
{
	for (const std::shared_ptr<Token>& token : tokens)
	{
		sf::Vector2f circleCenter = token->GetShape().getPosition();
		float radius = token->GetShape().getRadius();

		float dx = mousePosition.x - circleCenter.x;
		float dy = mousePosition.y - circleCenter.y;
		float distanceSquared = dx * dx + dy * dy;

		if (distanceSquared <= radius * radius)
		{
			if (ControlInteraction(token))
				ControlNextTurn(token);
		}
	}
}

bool Client::ControlInteraction(const std::shared_ptr<Token>& token)
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
				return true;
			}
			else
				return false;

		}
		//Mover ficha
		std::cout << "Mueves el token: " << diceValue << " casillas" << std::endl;
		diceValue = token->MoveToken(diceValue);
		return true;
	}
	else if ((diceValue == 6 || diceValue == 7) && HasTokensInSameCell())
	{
		if (token->GetCurrentCell()->GetTokensInCell() > 1)
		{
			std::cout << "Rompes la barrera" << std::endl;
			diceValue = token->MoveToken(diceValue);
			return true;
		}
		return false;
	}
	else
	{
		if (token->GetIsInGame())
		{
			//Mover ficha
			std::cout << "Mueves el token: " << diceValue << " casillas" << std::endl;
			diceValue = token->MoveToken(diceValue);
			return true;
		}
		return false;
	}
}

void Client::ControlNextTurn(const std::shared_ptr<Token>& token)
{
	if (diceValue == 6 || diceValue == 7)
	{
		canThrowDice = true;
		std::cout << "Vuelve a tirar" << std::endl;
		diceValue = 0;
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
				token->GetCurrentCell()->AddTokensInCell(-1);
				tokens.erase(it);
				if (tokens.empty())
				{
					std::cout << "Has ganado" << std::endl;
				}
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
		}
	}

	diceValue = 0;
	GAME.EndTurn();
}

void Client::Update(sf::RenderWindow& window)
{
	for (const std::shared_ptr<Token>& token : tokens)
	{
		token->Render(window);
	}
}

void Client::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		switch (mousePressed->button)
		{
		case sf::Mouse::Button::Left:
			if (canThrowDice)
			{
				diceValue = ThrowDice();
				ControlDice();
			}
			else
				SelectToken(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			break;
		}
	}

	if (const sf::Event::TextEntered* text = event.getIf<sf::Event::TextEntered>())
	{
		if (canThrowDice)
		{
			char c = static_cast<char>(text->unicode);

			if (std::isdigit(c))
			{
				diceValue += (c - '0');
			}
		}
	}

	if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (canThrowDice)
		{
			switch (keyPressed->code)
			{
			case sf::Keyboard::Key::Enter:
				ControlDice();
				break;
			}
		}
	}

}

std::string Client::GetColorString()
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

bool Client::AllTokensInBase()
{
	for (const std::shared_ptr<Token>& token : tokens)
		if (token->GetIsInGame())
			return false;

	return true;
}

bool Client::AnyTokenInBase()
{
	for (const std::shared_ptr<Token>& token : tokens)
		if (!token->GetIsInGame())
			return true;

	return false;
}

bool Client::HasTokensInSameCell() const
{
	for (int i = 0; i < tokens.size(); ++i) {
		for (int j = i + 1; j < tokens.size(); ++j) {
			if (tokens[i]->GetCurrentCell() == tokens[j]->GetCurrentCell()) {
				return true;
			}
		}
	}
	return false;
}