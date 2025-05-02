#include "SceneManager.h"

void SceneManager::InitScenes(Scene* scene)
{
	window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Tutorial SMFL",
		sf::Style::Titlebar | sf::Style::Close);
	currentScene = scene;
	currentScene->enter(*window);
}

void SceneManager::ChangeScene(Scene* scene)
{
	currentScene->exit();
	currentScene = scene;
	currentScene->enter(*window);
}

void SceneManager::Update()
{
	while (window->isOpen())
	{
		while (const std::optional event = window->pollEvent())
		{
			currentScene->update(*window, *event);
		}
	}

	delete window;
}
