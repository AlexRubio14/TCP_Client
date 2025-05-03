#include "RegisterScene.h"
#include "SceneManager.h"
#include "NetworkManager.h"

RegisterScene::RegisterScene() : Scene()
{

}

void RegisterScene::enter(sf::RenderWindow& window)
{
	std::cout << "Enter Register Scene" << std::endl;
	
	for (int i = 0; i < 2; i++)
	{
		CreateButtons(window, i);
		CreateTextField(window, i);
	}

	currentText = nullptr;
}

void RegisterScene::CreateButtons(sf::RenderWindow& window, int id)
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
		text = "Register";
	}
	else
	{
		position.x += 200;
		text = "Login";
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

void RegisterScene::CreateTextField(sf::RenderWindow& window, int id)
{
	textBackGround.push_back(sf::RectangleShape());
	textsInformation.push_back(sf::Text(font));
	texts.push_back(sf::Text(font));

	sf::Vector2f position(
		window.getSize().x / 2.f,
		window.getSize().y / 2.f - 100.f
	);
	std::string text;

	if (id == 0)
	{
		position.x -= 200;
		text = "Name";
	}
	else
	{
		position.x += 200;
		text = "Password";
	}

	textBackGround[id].setSize({ 250.f, 50.f });
	textBackGround[id].setFillColor(sf::Color::White);
	textBackGround[id].setOrigin(textBackGround[id].getSize() / 2.f);
	textBackGround[id].setPosition(position);

	textsInformation[id].setString(text);
	textsInformation[id].setFillColor(sf::Color::Black);

	texts[id].setFillColor(sf::Color::Black);

	sf::FloatRect textInformationBounds = textsInformation[id].getLocalBounds();
	textsInformation[id].setOrigin(sf::Vector2f(
		textInformationBounds.position.x + textInformationBounds.size.x / 2.f,
		textInformationBounds.position.y + textInformationBounds.size.y / 2.f + 45
	));

	sf::FloatRect textBounds = texts[id].getLocalBounds();
	texts[id].setOrigin(sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.f + 115,
		textBounds.position.y + textBounds.size.y / 2.f + 20
	));

	texts[id].setPosition(position);
	textsInformation[id].setPosition(position);
}

void RegisterScene::Render(sf::RenderWindow& window)
{
	sf::Color brown(238, 208, 157);

	window.clear(brown);
	for (sf::RectangleShape buton : buttons)
		window.draw(buton);
	for (sf::RectangleShape backGround : textBackGround)
		window.draw(backGround);
	for (sf::Text text : textsInformation)
		window.draw(text);
	for (sf::Text text : buttonsTexts)
		window.draw(text);
	for (sf::Text text : texts)
		window.draw(text);

	window.display();
}


void RegisterScene::DetectRectangle(sf::Vector2f mousePosition)
{
	for (size_t i = 0; i < textBackGround.size(); ++i) {
		if (textBackGround[i].getGlobalBounds().contains(mousePosition)) {
			inputText = std::string();
			currentText = &texts[i];
		}
	}

	for (size_t i = 0; i < buttons.size(); ++i) {
		if (buttons[i].getGlobalBounds().contains(mousePosition)) {
			currentText = nullptr;

			if (EmptyInformation())
				return;

			if (i == 0) 
			{
				//Mandar paquete con la información del nombre y password
				std::cout << "Register Done"<<std::endl;
				SCENE.ChangeScene(new LobbyScene());
			}
			else if (i == 1) 
			{
				//Mandar paquete con la información del nombre y password y mirar si puede logear
				SCENE.ChangeScene(new LobbyScene());
				std::cout << "Login Done" << std::endl;
			}
		}
	}
}

bool RegisterScene::EmptyInformation()
{
	for (sf::Text text : texts)
		if (text.getString().isEmpty())
			return true;
	return false;
}

void RegisterScene::exit()
{
	std::cout << "Exit Register Scene" << std::endl;
}

void RegisterScene::update(sf::RenderWindow& window, const sf::Event& event)
{
	HandleEvent(window, event);
	NETWORK.Update();
	Render(window);
}


