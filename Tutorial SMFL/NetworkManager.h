#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#include "CustomPacket.h"

#define NETWORK NetworkManager::Instance()

#define SERVER_PORT 55000
const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);

class NetworkManager
{
private:
	NetworkManager() = default;

	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator =(const NetworkManager&) = delete;

	sf::TcpSocket socketServer;
	CustomPacket customPacket;

	sf::TcpListener listener;
	sf::SocketSelector socketSelector;

public:
	inline static NetworkManager& Instance()
	{
		static NetworkManager manager;
		return manager;
	}

	bool ConnectServer();
	void DisconnectServer();

	void Update();
	void HandleNewConnection();
	void UpdateClient();
	void RecivePacket();

	inline sf::TcpSocket& GetSocketServer() { return socketServer; }
	inline sf::SocketSelector GetSocketSelector() { return socketSelector; }
	inline sf::TcpListener& GetListener() { return listener; }
};

