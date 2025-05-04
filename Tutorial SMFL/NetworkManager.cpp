#include "NetworkManager.h"
#include "PacketManager.h"

bool NetworkManager::ConnectServer()
{
	if (socketServer.connect(SERVER_IP, SERVER_PORT) == sf::Socket::Status::Done)
	{
		std::cout << "Connected" << std::endl;

		socketServer.setBlocking(false);  
		socketSelector.add(socketServer); 
		running = true; 

		networkThread = std::thread([this]() {
			while (running)
			{
				Update();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			});

		return true;
	}
	std::cerr << "Error trying connect the Server" << std::endl;
	return false;
}

void NetworkManager::ConnectClients(std::vector<std::shared_ptr<Client>> clients) 
{
	for (int i = 0; i < clients.size(); i++)
	{
		std::string ipStr = clients[i]->GetIp();
		auto resolved = sf::IpAddress::resolve(ipStr);

		if (resolved) {
			sf::IpAddress ip = *resolved;
			std::cout << "Resolved IP: " << ip.toString() << std::endl;

			if (socketServer.connect(ip, SERVER_PORT) == sf::Socket::Status::Done)
			{
				std::cout << "Connected to " << ip.toString() << std::endl;

				socketServer.setBlocking(false);
				socketSelector.add(socketServer);
			}
			else
			{
				std::cerr << "Error connecting to " << ip.toString() << std::endl;
			}
		}
		else {
			std::cerr << "Failed to resolve IP: " << ipStr << std::endl;
		}
	}

}


void NetworkManager::DisconnectServer()
{
	running = false;
	if (networkThread.joinable())
		networkThread.join();

	socketServer.disconnect();
	std::cout << "Disconnect Server" << std::endl;
}

void NetworkManager::Update()
{
	if (socketSelector.wait(sf::milliseconds(100)))
	{
		if (socketSelector.isReady(listener))
		{
			HandleNewConnection();
		}
		else
		{
			UpdateClient();
		}
		RecivePacket();
	}
}

void NetworkManager::HandleNewConnection()
{
	//Client& newClient;

	//if (listener.accept(newClient.GetSocket()) == sf::Socket::Status::Done)
	//{
	//	newClient.GetSocket().setBlocking(false);
	//	socketSelector.add(newClient.GetSocket());
	//}
}

void NetworkManager::UpdateClient()
{
	//for (std::shared_ptr<Client> client : pendingClients)
	//{
	//	if (client && _socketSelector.isReady(client->GetSocket()))
	//	{
	//		client->HandleIncomingPackets();
	//	}
	//	else if (!client)
	//	{
	//		std::cerr << "Invalid client pointer detected." << std::endl;
	//	}
	//}
}

void NetworkManager::RecivePacket()
{
	if (socketServer.receive(customPacket.packet) == sf::Socket::Status::Done)
	{
		PACKET_MANAGER.ProcessPacket(" ", customPacket);
	}
	else
	{
		std::cout << "Packet not received" << std::endl;
	}
}
