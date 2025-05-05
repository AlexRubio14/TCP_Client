#include "NetworkManager.h"
#include "PacketManager.h"
#include "GameManager.h"


NetworkManager::NetworkManager()
{
	listeningPort = 55001;
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

		socketState = Connected;

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

void NetworkManager::StartListeningForClients(const int numPort) {
	if (listener.listen(numPort) == sf::Socket::Status::Done) {
		std::cout << "Listening for incoming connections on port " << numPort << std::endl;
	}
	else {
		std::cerr << "Error: Unable to start listener on port " << numPort << std::endl;
	}
}

void NetworkManager::StartClientConnections(std::vector<std::shared_ptr<Client>> clients, const int myIndex, const int myPort) {
	if (clients.size() <= 0)
		return;

	
	clientThread = std::thread([this, clients, myIndex, myPort]() {

		try
		{
			sf::SocketSelector clientSelector;

			StartListeningForClients(myPort); //Start lsitening in my port\

			runningClients = true;
			listener.setBlocking(false);

			for (int i = 0; i < clients.size(); i++)
			{
				if (i == myIndex)
					continue;

				std::string ipStr = clients[i]->GetIp();
				int port = clients[i]->GetNumPort();

				std::optional<sf::IpAddress> resolved = sf::IpAddress::resolve(ipStr);

				if (resolved)
				{
					sf::IpAddress ip = *resolved;
					std::cout << "conectadno a: " << ip.toString() << ":" << port << std::endl;

					sf::Socket::Status status = clients[i]->GetSocket().connect(ip, port);
					if (status == sf::Socket::Status::Done) {
						std::cout << "Connected to " << ip.toString() << std::endl;
						clients[i]->GetSocket().setBlocking(false);
						clientSelector.add(clients[i]->GetSocket());

						sf::Packet packet;
						packet << "ping";
						clients[i]->GetSocket().send(packet);
					}
					else {
						std::cerr << "Failed to connect to " << ip.toString() << ", Error: " << static_cast<int>(status) << std::endl;
					}
				}
				else
				{
					std::cerr << "Failed to resolve IP: " << ipStr << std::endl;
				}
			}


			while (runningClients) {
				std::cout << "Estoy runeando" << std::endl;
				sf::TcpSocket tempSocket;

				if (listener.accept(tempSocket) == sf::Socket::Status::Done) {
					std::string clientIp = tempSocket.getRemoteAddress()->toString();
					std::cout << "New client connected: " << clientIp << std::endl;

					std::shared_ptr<Client> matchingClient = nullptr;

					for (std::shared_ptr<Client> client : clients)
					{
						if (client->GetIp() == clientIp) {
							matchingClient = client;
							break;
						}
					}


					if (matchingClient)
					{
						std::shared_ptr<sf::TcpSocket> newSocket = std::make_shared<sf::TcpSocket>(std::move(tempSocket));
						newSocket->setBlocking(false);
						clientSelector.remove(matchingClient->GetSocket());
						matchingClient->SetSocket(std::move(newSocket));


						if (matchingClient->GetSocket().getRemoteAddress()->toString() != clientIp) {
							std::cerr << "Warning: socket mismatch after SetSocket!" << std::endl;
						}

						clientSelector.add(matchingClient->GetSocket());
						std::cout << "Client connected: " << matchingClient->GetIp() << std::endl;
					}
					else {
						std::cerr << "Client not found in the list: " << clientIp << std::endl;
					}

					if (clientSelector.wait())
					{
						std::cout << "Selector ready, checking clients..." << std::endl;
						UpdateClients(clientSelector);

					}
					else {
						std::cout << "Selector timed out with no activity." << std::endl;
					}
				}
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Excepción en el hilo: " << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Excepción desconocida en el hilo" << std::endl;
		}
		
	});

	clientThread.detach();
}


void NetworkManager::DisconnectServer()
{
	std::lock_guard<std::mutex> lock(networkMutex);
	if (IsConnected()) {
		socketSelector.remove(socketServer);
		socketServer.disconnect();
		running = false;
		std::cout << "Server disconnected successfully" << std::endl;
	}
	else {
		std::cout << "Server is already disconnected." << std::endl;
	}
}

void NetworkManager::DisconnectClient()
{
	//runningClients = false;

	for (int i = 0; i < GAME.GetClients().size(); i++)
		GAME.GetClients()[i]->GetSocket().disconnect();
}

void NetworkManager::Update()
{
	if (socketSelector.wait())
	{
		if (socketSelector.isReady(socketServer))
		{
			RecivePacket();
		}
	}
}

void NetworkManager::UpdateClients(sf::SocketSelector& clientSelector)
{
	std::cout << "Clients vector size: " << GAME.GetClients().size() << std::endl;

	for (std::shared_ptr<Client> client : GAME.GetClients())
	{
		if (client && clientSelector.isReady(client->GetSocket()))
		{
			std::cout << "estoy recibiendo un paquete" << std::endl;
			client->HandleIncomingPackets();
		}
		else if (!client)
		{
			std::cerr << "Invalid client pointer detected." << std::endl;
		}
	}
}

void NetworkManager::RecivePacket()
{
	if (socketServer.receive(customPacket.packet) == sf::Socket::Status::Done)
	{
		PACKET_MANAGER.ProcessPacket(" ", customPacket);
	}
	else
	{
		std::cout << "Packet not received Server" << std::endl;
	}
}

void NetworkManager::RecivePacketClient(std::shared_ptr<Client> client)
{
	CustomPacket customPacketClient;
	if (client->GetSocket().receive(customPacketClient.packet) == sf::Socket::Status::Done)
	{
		PACKET_MANAGER.ProcessPacket(" ", customPacketClient);
	}
	else
	{
		std::cout << "Packet not received Client" << std::endl;
	}
}

bool NetworkManager::IsConnected() const {
	return socketState == Connected;
}