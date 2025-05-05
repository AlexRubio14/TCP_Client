#include "SceneManager.h"
#include "PacketManager.h"
#include "NetworkManager.h"

void SceneManager::InitScenes(Scene* scene)
{
	window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Tutorial SMFL",
		sf::Style::Titlebar | sf::Style::Close);
	currentScene = scene;
	currentScene->Enter(*window);

	PACKET_MANAGER.Init();
	PACKET_MANAGER.SendHandshake(" ");
	NETWORK.GetSocketSelector().add(NETWORK.GetListener());
}

void SceneManager::ChangeScene(Scene* scene)
{
	currentScene->Exit();
	currentScene = scene;
	currentScene->Enter(*window);
}

void SceneManager::Update()
{
	while (window->isOpen())
	{
		while (const std::optional event = window->pollEvent())
		{
			currentScene->Update(*window, *event);
		}
	}

	delete window;
}
