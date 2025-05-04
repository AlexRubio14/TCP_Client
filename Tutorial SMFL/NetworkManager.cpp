#include "NetworkManager.h"
#include "PacketManager.h"
#include "GameManager.h"


NetworkManager::NetworkManager()
{
	listeningPort = 55004;
}
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

void NetworkManager::ConnectClients(std::vector<std::shared_ptr<Client>> clients) {
	for (int i = 0; i < clients.size(); i++) {
		std::string ipStr = clients[i]->GetIp();
		std::optional<sf::IpAddress> resolved = sf::IpAddress::resolve(ipStr);

		if (resolved) {
			sf::IpAddress ip = *resolved;
			std::cout << "Resolved IP: " << ip.toString() << std::endl;

			sf::Socket::Status status = clients[i]->GetSocket().connect(ip, listeningPort + clients[i]->GetIndex());

			if (status == sf::Socket::Status::Done) {
				std::cout << "Connected to " << ip.toString() << std::endl;
				clients[i]->GetSocket().setBlocking(false);
				socketSelector.add(clients[i]->GetSocket());
			}
			else {
				std::cerr << "Failed to connect to " << ip.toString() << ", Error: " << static_cast<int>(status) << std::endl;
			}
		}
		else {
			std::cerr << "Failed to resolve IP: " << ipStr << std::endl;
		}
	}
}

void NetworkManager::StartListeningForClients(sf::TcpListener& listener, const int numPort) {
	if (listener.listen(numPort) == sf::Socket::Status::Done) {
		std::cout << "Listening for incoming connections on port " << numPort << std::endl;
	}
	else {
		std::cerr << "Error: Unable to start listener on port " << numPort << std::endl;
	}
}

void NetworkManager::StartClientConnections(std::vector<std::shared_ptr<Client>> clients, const int numPort) {
	std::thread listenerThread([this, clients, numPort]() {
		StartListeningForClients(listener, numPort);
		runningClients = true;
		while (runningClients) {

			Update();
			std::shared_ptr<Client> matchingClient = nullptr;
			sf::TcpSocket tempSocket;

			if (listener.accept(tempSocket) == sf::Socket::Status::Done) {
				std::optional<sf::IpAddress> clientIp = tempSocket.getRemoteAddress();
				std::cout << "New client connected: " << clientIp->toString() << std::endl;

				for (std::shared_ptr<Client> client : clients)
				{
					if (client->GetIp() == clientIp->toString())
					{
						matchingClient = client;
						break;
					}
				}

				if (matchingClient != nullptr)
				{
					matchingClient->GetSocket() = std::move(tempSocket);
					matchingClient->GetSocket().setBlocking(false);
					socketSelector.add(matchingClient->GetSocket());

					std::cout << "Client connected: " << matchingClient->GetIp() << std::endl;
				}
				else
				{
					std::cerr << "Client not found in the list: " << clientIp->toString() << std::endl;
				}
			}
		}
		});

	std::thread connectThread([this, clients]() {
		ConnectClients(clients);
		});

	listenerThread.detach();
	connectThread.detach();
}


void NetworkManager::DisconnectServer()
{
	running = false;
	if (networkThread.joinable())
		networkThread.join();

	socketServer.disconnect();
	std::cout << "Disconnect Server" << std::endl;
}

void NetworkManager::DisconnectClient()
{
	runningClients = false;
	for (int i = 0; i < GAME.GetClients().size(); i++)
		GAME.GetClients()[i]->GetSocket().disconnect();
}

void NetworkManager::Update()
{
	if (socketSelector.wait(sf::milliseconds(100)))
	{
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
	std::cout << "Entra" << std::endl;
	if (socketServer.receive(customPacket.packet) == sf::Socket::Status::Done)
	{
		PACKET_MANAGER.ProcessPacket(" ", customPacket);
	}
	else
	{
		std::cout << "Packet not received" << std::endl;
	}
}
