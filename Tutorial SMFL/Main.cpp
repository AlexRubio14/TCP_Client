#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#include "SceneManager.h"

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
	srand(time(NULL));
	SCENE.InitScenes(new RegisterScene());
	SCENE.Update();
}