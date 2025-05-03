#include "LobbyScene.h"
#include "SceneManager.h"
#include "NetworkManager.h"

LobbyScene::LobbyScene() : Scene()
{

}

void LobbyScene::enter(sf::RenderWindow& window)
{
	std::cout << "Enter Lobby Scene" << std::endl;

	for (int i = 0; i < 2; i++)
	{
		CreateButtons(window, i);
	}
	CreateTextField(window);

	currentText = nullptr;
}

void LobbyScene::exit()
{
	std::cout << "Exit Lobby Scene" << std::endl;
}

void LobbyScene::update(sf::RenderWindow& window, const sf::Event& event)
{
	HandleEvent(window, event);
	NETWORK.Update();
	Render(window);
}

void LobbyScene::DetectRectangle(sf::Vector2f mousePosition)
{
	if (textBackGround.getGlobalBounds().contains(mousePosition)) {
		inputText = std::string();
		currentText = textId;
	}

	for (size_t i = 0; i < buttons.size(); ++i) {
		if (buttons[i].getGlobalBounds().contains(mousePosition)) {
			currentText = nullptr;

			if (textId->getString().isEmpty())
				return;

			if (i == 0)
			{
				//Mandar paquete con la información de la ID
				std::cout << "Create Room" << std::endl;
				SCENE.ChangeScene(new GameScene());
			}
			else if (i == 1)
			{
				//Mandar paquete con la información de la ID y mirar si hay sala
				SCENE.ChangeScene(new GameScene());
				std::cout << "Join Room" << std::endl;
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
	));;

	buttonsTexts[id].setPosition(position);
}

void LobbyScene::CreateTextField(sf::RenderWindow& window)
{
	textBackGround = sf::RectangleShape();
	textsInformation = new sf::Text(font);
	textId = new sf::Text(font);

	sf::Vector2f position(
		window.getSize().x / 2.f,
		window.getSize().y / 2.f - 100.f
	);

	textBackGround.setSize({ 250.f, 50.f });
	textBackGround.setFillColor(sf::Color::White);
	textBackGround.setOrigin(textBackGround.getSize() / 2.f);
	textBackGround.setPosition(position);

	textsInformation->setString("ID");
	textsInformation->setFillColor(sf::Color::Black);

	textId->setFillColor(sf::Color::Black);

	sf::FloatRect textInformationBounds = textsInformation->getLocalBounds();
	textsInformation->setOrigin(sf::Vector2f(
		textInformationBounds.position.x + textInformationBounds.size.x / 2.f,
		textInformationBounds.position.y + textInformationBounds.size.y / 2.f + 45
	));

	sf::FloatRect textBounds = textId->getLocalBounds();
	textId->setOrigin(sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.f + 115,
		textBounds.position.y + textBounds.size.y / 2.f + 20
	));

	textId->setPosition(position);
	textsInformation->setPosition(position);
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
	window.draw(*textsInformation);
	window.draw(*textId);

	window.display();
}
