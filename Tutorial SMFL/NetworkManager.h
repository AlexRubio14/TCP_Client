#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include "Client.h"

#include "CustomPacket.h"
#include <thread>

#define NETWORK NetworkManager::Instance()

#define SERVER_PORT 55000
const sf::IpAddress SERVER_IP = sf::IpAddress(93,176,163,135);
//const sf::IpAddress SERVER_IP = sf::IpAddress(127,0,0,1);

class NetworkManager
{
private:
	NetworkManager();

	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator =(const NetworkManager&) = delete;

	sf::TcpSocket socketServer;
	CustomPacket customPacket;

	sf::TcpListener listener;
	sf::SocketSelector socketSelector;

	std::thread networkThread;
	std::atomic<bool> running = false;

	int listeningPort;

public:
	inline static NetworkManager& Instance()
	{
		static NetworkManager manager;
		return manager;
	}

	bool ConnectServer();
	void ConnectClients(std::vector<std::shared_ptr<Client>> clients);
	void StartClientConnections(std::vector<std::shared_ptr<Client>> clients, const int numPort);
	void StartListeningForClients(sf::TcpListener& listener, const int numPort);
	void DisconnectServer();
	void DisconnectClient();

	void Update();
	void HandleNewConnection();
	void UpdateClient();
	void RecivePacket();

	inline sf::TcpSocket& GetSocketServer() { return socketServer; }
	inline sf::SocketSelector GetSocketSelector() { return socketSelector; }
	inline sf::TcpListener& GetListener() { return listener; }
	inline int GetListeningPort() const { return listeningPort; }
};

