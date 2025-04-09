#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#include "Map.h"

#define SERVER_PORT 55000
#define WIDTH 850
#define HEIGHT 850

const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);

enum TipoPaquete { HANDSHAKE, LOGIN, MOVIMIENTO };

sf::Packet& operator >>(sf::Packet& packet, TipoPaquete& tipo)
{
	int temp;
	packet >> temp;
	tipo = static_cast<TipoPaquete>(temp);

	return packet;
}
void HandShake(sf::Packet& data)
{
	std::string message;
	data >> message;

	std::cout << "Mensaje recibido del servidor: " << message << std::endl;
}

 // ----------------------------------- Client -----------------------------------

void main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({WIDTH, HEIGHT}), "Tutorial SMFL");

	Map* map = new Map(*window);
	map->PrintMap();

	while (window->isOpen())
	{
		map->Update(*window);
	}


	delete window;


	//sf::TcpSocket socketServer;

	//sf::Packet packet;
	//bool gameLoop = true;
	//if (socketServer.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	//{
	//	std::cerr << "Error al conectar con el servidor" << std::endl;
	//	return;
	//}

	//std::cout << "Conectado con el servidor" << std::endl;

	//while (gameLoop)
	//{
	//	
	//	/*if (socketServer.receive(packet) == sf::Socket::Status::Done)
	//	{
	//		std::string receivedMessage;

	//		TipoPaquete type;

	//		packet >> type;

	//		switch (type)
	//		{
	//		case HANDSHAKE:
	//			HandShake(packet);
	//			break;
	//		case LOGIN:
	//			break;
	//		case MOVIMIENTO:
	//			break;
	//		default:
	//			break;
	//		}

	//		packet.clear();

	//		std::cout << "Mensaje recibido del servidor: " << receivedMessage << std::endl;
	//	}*/
	//}

	//socketServer.disconnect();
	//std::cout << "Desconectado del servidor" << std::endl;
}