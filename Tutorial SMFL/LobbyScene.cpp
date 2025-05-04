#include "LobbyScene.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "EventManager.h"

LobbyScene::LobbyScene() : Scene()
{

}

void LobbyScene::Enter(sf::RenderWindow& window)
{
	std::cout << "Enter Lobby Scene" << std::endl;

	CreateTextField(window);
	for (int i = 0; i < 2; i++)
	{
		CreateButtons(window, i);
	}


	currentText = nullptr;
}

void LobbyScene::Exit()
{
	std::cout << "Exit Lobby Scene" << std::endl;
}

void LobbyScene::Update(sf::RenderWindow& window, const sf::Event& event)
{
	HandleEvent(window, event);
	Render(window);
}

void LobbyScene::DetectRectangle(sf::Vector2f mousePosition)
{
	if (textBackGround.getGlobalBounds().contains(mousePosition)) {
		inputText = std::string();
		currentText = &idInformation[1];
	}

	for (size_t i = 0; i < buttons.size(); ++i) {
		if (buttons[i].getGlobalBounds().contains(mousePosition)) {
			currentText = nullptr;

			if (idInformation[1].getString().isEmpty())
				return;

			if (i == 0)
			{
				//Mandar paquete con la información de la ID
				CustomPacket customPacket(CREATE_ROOM);
				customPacket.packet << idInformation[1].getString().toAnsiString();

				EVENT_MANAGER.Emit(CREATE_ROOM, " ", customPacket);
				std::cout << "Create room Send" << std::endl;
			}
			else if (i == 1)
			{
				CustomPacket customPacket(JOIN_ROOM);
				customPacket.packet << idInformation[1].getString().toAnsiString();

				EVENT_MANAGER.Emit(JOIN_ROOM, " ", customPacket);
				std::cout << "Join room Send" << std::endl;
			}
		}
	}
}


void LobbyScene::CreateButtons(sf::RenderWindow& window, int id)
{
	buttons.push_back(sf::RectangleShape());
	buttonsTexts.push_back(sf::Text(font));

	sf::Vector2f position(
		window.getSize().x / 2.f,
		window.getSize().y / 2.f + 200.f
	);
	std::string text;

	if (id == 0)
	{
		position.x -= 200;
		text = "Create Room";
	}
	else
	{
		position.x += 200;
		text = "Join Room";
	}

	buttons[id].setSize({ 300.f, 75.f });
	buttons[id].setFillColor(sf::Color::Green);
	buttons[id].setOrigin(buttons[id].getSize() / 2.f);
	buttons[id].setPosition(position);

	buttonsTexts[id].setString(text);
	buttonsTexts[id].setFillColor(sf::Color::Black);

	sf::FloatRect textBounds = buttonsTexts[id].getLocalBounds();
	buttonsTexts[id].setOrigin(sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.f,
		textBounds.position.y + textBounds.size.y / 2.f
	));

	buttonsTexts[id].setPosition(position);
}

void LobbyScene::CreateTextField(sf::RenderWindow& window)
{
	textBackGround = sf::RectangleShape();

	for (int i = 0; i < 2; i++)
	{
		idInformation.push_back(sf::Text(font));
	}

	sf::Vector2f position(
		window.getSize().x / 2.f,
		window.getSize().y / 2.f - 100.f
	);

	textBackGround.setSize({ 250.f, 50.f });
	textBackGround.setFillColor(sf::Color::White);
	textBackGround.setOrigin(textBackGround.getSize() / 2.f);
	textBackGround.setPosition(position);

	idInformation[0].setString("ID");
	idInformation[0].setFillColor(sf::Color::Black);

	idInformation[1].setFillColor(sf::Color::Black);

	sf::FloatRect textInformationBounds = idInformation[0].getLocalBounds();
	idInformation[0].setOrigin(sf::Vector2f(
		textInformationBounds.position.x + textInformationBounds.size.x / 2.f,
		textInformationBounds.position.y + textInformationBounds.size.y / 2.f + 45
	));

	sf::FloatRect textBounds = idInformation[1].getLocalBounds();
	idInformation[1].setOrigin(sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.f + 115,
		textBounds.position.y + textBounds.size.y / 2.f + 20
	));

	idInformation[1].setPosition(position);
	idInformation[0].setPosition(position);
}

void LobbyScene::Render(sf::RenderWindow& window)
{
	sf::Color brown(238, 208, 157);

	window.clear(brown);

	for (sf::RectangleShape buton : buttons)
		window.draw(buton);
	window.draw(textBackGround);
	for (sf::Text text : buttonsTexts)
		window.draw(text);

	for (sf::Text text : idInformation)
		window.draw(text);

	window.display();
}
