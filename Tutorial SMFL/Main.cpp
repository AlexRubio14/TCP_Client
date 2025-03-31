#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#define SERVER_PORT 55000

const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);

enum TipoPaquete { HANDSHAKE, LOGIN, REGISTER, MOVIMIENTO, TEST, WAIT };

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

	sf::Packet packet;
	bool gameLoop = true;

	if (socketServer.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al conectar con el servidor" << std::endl;
		return;
	}

	std::cout << "Conectado con el servidor" << std::endl;

	packet << TipoPaquete::TEST << "Hola servidor";

	if (socketServer.send(packet) == sf::Socket::Status::Done)
	{
		std::cout << "He saludado al servidor " << std::endl;
	}
	else
	{
		std::cout << "No he podido saludar al server " << std::endl;
	}

	while (gameLoop)
	{
		if (socketServer.receive(packet) == sf::Socket::Status::Done)
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
			case REGISTER:
				break;
			case MOVIMIENTO:
				break;
			case TEST:
				break;
			case WAIT:
				break;
			default:
				break;
			}

			packet.clear();

			std::cout << "Mensaje recibido del servidor: " << receivedMessage << std::endl;
		}
	}

	socketServer.disconnect();
	std::cout << "Desconectado del servidor" << std::endl;
}

/*std::string message;
		std::cout << "Inserta mensaje para el servidor, -1 Para salir" << std::endl;
		std::cin >> message;

		if (message == "-1")
		{
			std::cout << "Desconectando..." << std::endl;
			gameLoop = false;
		}
		else
		{
			sf::Packet packet;
			packet << message;
			if (socketServer.send(packet) != sf::Socket::Status::Done)
			{
				std::cerr << "Error al enviar el paquete al servidor" << std::endl;
			}
			else
			{
				std::cout << "Mensaje enviado: " << message << std::endl;
			}
		}*/