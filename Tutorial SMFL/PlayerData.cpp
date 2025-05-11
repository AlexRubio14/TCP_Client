#include "PlayerData.h"
#include "GameManager.h"
#include <iostream>
#include "CustomPacket.h"
#include "EventManager.h"
#include "NetworkManager.h"

PlayerData::PlayerData(const std::string& username, const sf::Color& color, int index)
	: username(username), color(color), index(index), extraMoves(false), canThrowDice(false), diceValue(0)
{
	int idToSpawn = SPAWN_BLUE;
	if (color == sf::Color::Yellow)
		idToSpawn += COLORED_CELLS_PER_COLOR;
	else if (color == sf::Color::Red)
		idToSpawn += (COLORED_CELLS_PER_COLOR * 2);
	else if (color == sf::Color::Green)
		idToSpawn += (COLORED_CELLS_PER_COLOR * 3);

	for (int i = 0; i < 4; i++)
	{
		std::shared_ptr<Token> token = std::make_shared<Token>(GAME.GetMap()->GetCells()[idToSpawn + i], color,i);
		tokens.push_back(token);
	}
}

int PlayerData::ThrowDice()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	return (std::rand() % 6) + 1;
}

void PlayerData::ControlDice()
{
	SendDiceValue();
	if (AllTokensInBase() && diceValue != 5)
	{
		if (diceValue == 6)
		{
			canThrowDice = true;
			diceValue = 0;
			return;
		}
		GAME.EndTurn();
		return;
	}
	canThrowDice = false;
	if (!AnyTokenInBase() && diceValue == 6)
		diceValue = 7;
}

void PlayerData::SelectToken(sf::Vector2f mousePosition)
{
	for (const std::shared_ptr<Token>& token : tokens)
	{
		sf::Vector2f circleCenter = token->GetShape().getPosition();
		float radius = token->GetShape().getRadius();

		float dx = mousePosition.x - circleCenter.x;
		float dy = mousePosition.y - circleCenter.y;
		float distanceSquared = dx * dx + dy * dy;

		if (distanceSquared <= radius * radius && token->GetIsInGame() != END_GAME)
		{
			if (ControlInteraction(token))
				ControlNextTurn(token);
		}
	}
}

bool PlayerData::ControlInteraction(const std::shared_ptr<Token>& token)
{
	if (diceValue == 5)
	{
		if (AnyTokenInBase())
		{
			if (token->GetIsInGame() == IN_BASE)
			{
				//Forzar sacar ficha si sale 5 y hay fichas en base
				//std::cout << "Sacas la casilla de base" << std::endl;
				SendMoveToken(1, token->GetId());
				diceValue = token->MoveToken(1);
				return true;
			}
			else
				return false;

		}
		//Mover ficha
		//std::cout << "Mueves el token: " << diceValue << " casillas" << std::endl;
		SendMoveToken(diceValue, token->GetId());
		diceValue = token->MoveToken(diceValue);
		return true;
	}
	else if ((diceValue == 6 || diceValue == 7) && HasTokenInSameCell())
	{
		if (token->GetCurrentCell()->GetTokensInCell() > 1)
		{
			//std::cout << "Rompes la barrera" << std::endl;
			SendMoveToken(diceValue, token->GetId());
			diceValue = token->MoveToken(diceValue);
			return true;
		}
		return false;
	}
	else
	{
		if (token->GetIsInGame() == IN_GAME)
		{
			//Mover ficha
			//std::cout << "Mueves el token: " << diceValue << " casillas" << std::endl;
			SendMoveToken(diceValue, token->GetId());
			diceValue = token->MoveToken(diceValue);
			return true;
		}
		return false;
	}
}

void PlayerData::ControlNextTurn(const std::shared_ptr<Token>& token)
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

void PlayerData::SendDiceValue()
{
	CustomPacket packet(PROCESS_DICE_VALUE);
	std::cout << "Dice Value: " << diceValue << std::endl;
	for (int i = 0; i < NETWORK.GetClients().size(); i++)
	{
		if (GAME.GetReferenceClient()->GetPlayerData().GetIndex() == NETWORK.GetClients()[i]->GetPlayerData().GetIndex())
			continue;

		packet.packet << diceValue;

		PACKET_MANAGER.SendPacketToClient(NETWORK.GetClients()[i], packet);
	}
}

void PlayerData::SendMoveToken(int value, int tokenID)
{
	CustomPacket packet(MOVE_TOKEN);
	std::cout << "Player with Color: "<< GetColorString() << ", Move Token with ID: " << tokenID << ", " << value << " Cells" << std::endl;
	for (int i = 0; i < NETWORK.GetClients().size(); i++)
	{
		if (GAME.GetReferenceClient()->GetPlayerData().GetIndex() == NETWORK.GetClients()[i]->GetPlayerData().GetIndex())
			continue;

		packet.packet << value << tokenID << GetColorString();

		PACKET_MANAGER.SendPacketToClient(NETWORK.GetClients()[i], packet);
	}
}



std::string PlayerData::GetColorString()
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

void PlayerData::Update(sf::RenderWindow& window)
{
	for (const std::shared_ptr<Token>& token : tokens)
	{
		token->Render(window);
	}
}

void PlayerData::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
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

void PlayerData::EraseToken(int index)
{
	for (auto it = tokens.begin(); it != tokens.end(); ++it)
	{
		if ((*it)->GetId() == index)
		{
			(*it)->ChangeTokenState(END_GAME);
			if (AllTokensEndGame())
				std::cout << "Has ganado" << std::endl;
			break;
		}
	}
}

bool PlayerData::AllTokensInBase() const
{
	for (const std::shared_ptr<Token>& token : tokens)
		if (token->GetIsInGame() == IN_GAME)
			return false;

	return true;
}

bool PlayerData::AnyTokenInBase() const
{
	for (const std::shared_ptr<Token>& token : tokens)
		if (token->GetIsInGame() == IN_BASE)
			return true;

	return false;
}

bool PlayerData::AllTokensEndGame() const
{
	for (const std::shared_ptr<Token>& token : tokens)
		if (token->GetIsInGame() != END_GAME)
			return false;

	return true;
}

bool PlayerData::HasTokenInSameCell()
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
