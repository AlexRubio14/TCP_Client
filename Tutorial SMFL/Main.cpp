#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#define SERVER_PORT 55000

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
	sf::TcpSocket socketServer;

	if (socketServer.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al conectar con el servidor" << std::endl;
		return;
	}
	std::cout << "Conexion Al servidor" << std::endl;

	sf::Packet packet;
	bool gameLoop = true;
	if (socketServer.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al conectar con el servidor" << std::endl;
		return;
	}

	std::cout << "Conectado con el servidor" << std::endl;

	while (gameLoop)
	{
		std::string message;
		std::cout << "Inserta mensaje para el servidor, -1 Para salir" << std::endl;
		std::cin >> message;

		if (message == "-1")
		{
			std::cout << "Desocnectando..." << std::endl;
			gameLoop = false;
		}
		else
		{
			sf::Packet packet;
			packet << message;
			if (socketServer.send(packet) != sf::Socket::Status::Done) 
			{
				std::cerr << "Error al enviar el pauqete al servidor" << std::endl;
			}
			else
			{
				std::cout << "Mensaje enviado: " << message << std::endl;
			}
		}
		/*if (socketServer.receive(packet) == sf::Socket::Status::Done)
		{
			std::string receivedMessage;

			TipoPaquete type;

			packet >> type;

			switch (type)
			{
			case HANDSHAKE:
				HandShake(packet);
				break;
			case LOGIN:
				break;
			case MOVIMIENTO:
				break;
			default:
				break;
			}

			packet.clear();

			std::cout << "Mensaje recibido del servidor: " << receivedMessage << std::endl;
		}*/
	}

	socketServer.disconnect();
	std::cout << "Desconectado del servidor" << std::endl;
}