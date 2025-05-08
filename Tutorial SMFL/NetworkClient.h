#pragma once
#include <SFML/Network.hpp>
#include <memory>
#include <string>

class NetworkClient
{
private:

	std::string ip;
	int port;
	std::shared_ptr<sf::TcpSocket> socket;

public:

	NetworkClient(const std::string& ip, int port);

	void HandleIncomingPackets();

	std::string& GetIp() { return ip; }
	int GetPort() const { return port; }
	sf::TcpSocket& GetSocket() { return *socket; }
	void SetSocket(const std::shared_ptr<sf::TcpSocket> newSocket) { socket = newSocket; }
};

