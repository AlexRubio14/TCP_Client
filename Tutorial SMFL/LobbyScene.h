#pragma once
#include "Scene.h"

class LobbyScene : public Scene
{
private:
    std::vector<sf::RectangleShape> buttons;
    sf::RectangleShape textBackGround;
    std::vector<sf::Text> buttonsTexts;
    std::vector<sf::Text> idInformation;

public:
    LobbyScene();
    void enter(sf::RenderWindow& window) override;
    void exit() override;
    void update(sf::RenderWindow& window, const sf::Event& event) override;
    void DetectRectangle(sf::Vector2f mousePosition) override;
    void CreateButtons(sf::RenderWindow& window, int id);
    void CreateTextField(sf::RenderWindow& window);
    void Render(sf::RenderWindow& window);
};

