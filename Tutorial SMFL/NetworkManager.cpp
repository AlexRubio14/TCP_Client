#include "NetworkManager.h"
#include "PacketManager.h"
#include "GameManager.h"
#include <optional>



void NetworkManager::HandleServerCommunication()
{
	NetworkState state;

	stateMutex.lock();
	state = currentState;
	stateMutex.unlock();

	while (state == NetworkState::CONNECTED_TO_SERVER)
	{

		if (socketSelector.wait(sf::seconds(0.1f)))
		{
			if (socketSelector.isReady(*serverSocket))
			{
				CustomPacket customPacket;

				sf::Socket::Status status = serverSocket->receive(customPacket.packet);

				if (status == sf::Socket::Status::Done)
				{
					PACKET_MANAGER.ProcessReceivedPacket(customPacket);
				}
				else
				{
					std::cerr << "Error receiving the packet: " << static_cast<int>(status) << std::endl;
				}
			}
		}
	}
}

void NetworkManager::HandleP2PCommunication()
{
	NetworkState state;

	stateMutex.lock();
	state = currentState;
	stateMutex.unlock();

	while (state == NetworkState::CONNECTED_TO_PEERS)
	{
		if (socketSelector.wait(sf::seconds(0.1f)))
		{
			for (std::shared_ptr<Client> client : p2pClients)
			{
				if (client && client->GetNetwork().GetSocket().getRemoteAddress() != sf::IpAddress::Any)
				{
					sf::TcpSocket& socket = client->GetNetwork().GetSocket();

					if (socketSelector.isReady(socket))
					{
						CustomPacket customPacket;
						sf::Socket::Status status = client->GetNetwork().GetSocket().receive(customPacket.packet);

						if (status == sf::Socket::Status::Done)
						{
							PACKET_MANAGER.ProcessReceivedPacket(customPacket);
						}
						else
						{
							std::cerr << "Error receiving the packet: " << static_cast<int>(status) << std::endl;

						}
					}
				}
			}
		}
	}
}

NetworkManager::~NetworkManager()
{
	Stop();
}

void NetworkManager::Init()
{
	serverSocket = std::make_shared<sf::TcpSocket>();
	serverIp = SERVER_IP;
	serverPort = SERVER_PORT;
	currentState = NetworkState::DISCONNECTED;
	isRunning = false;
}

void NetworkManager::Start()
{
	isRunning = true;
	networkThread = std::thread([this]() {
		while (isRunning) 
		{
			Update();
		}
		});
}

void NetworkManager::Update()
{
	NetworkState state;

	stateMutex.lock();
	state = currentState;
	stateMutex.unlock();

	switch (state) {
	case NetworkState::CONNECTED_TO_SERVER:
		HandleServerCommunication();
		break;
	case NetworkState::CONNECTED_TO_PEERS:
		HandleP2PCommunication();
		break;
	default:
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		break;
	}
}

void NetworkManager::Stop()
{
	isRunning = false;
	if (networkThread.joinable())
		networkThread.join();

	DisconnectServer();
	DisconnectAllPeers();
}

void NetworkManager::ChangeState(NetworkState newState)
{
	stateMutex.lock();
	currentState = newState;
	RefreshSelector();
	stateMutex.unlock();
}

void NetworkManager::StartListening()
{
	if (listener.listen(0) == sf::Socket::Status::Done)
		std::cout << "Listening on port: " << listener.getLocalPort() << std::endl;
	else
		std::cerr << "Failed to start Listening" << std::endl;
}

void NetworkManager::StartClientConnections(const std::vector<std::shared_ptr<Client>>& newClients, int myIndex, int port)
{
	p2pClients = newClients;

	for (int i = 0; i < p2pClients.size(); ++i)
	{
		if (i == myIndex) 
			continue;

		NetworkClient newClient = p2pClients[i]->GetNetwork();
		std::optional<sf::IpAddress> ipAdress = sf::IpAddress::resolve(newClient.GetIp());

		if (!ipAdress)
			continue;

		sf::Socket::Status status = newClient.GetSocket().connect(*ipAdress, newClient.GetPort());

		if (status == sf::Socket::Status::Done)
		{
			newClient.GetSocket().setBlocking(false);
			std::cout << "Connected to peer " << newClient.GetIp() << ":" << newClient.GetPort() << "\n";
		}
		else
		{
			std::cerr << "Failed to connect to peer " << newClient.GetIp() << "\n";
		}
	}

	ChangeState(NetworkState::CONNECTED_TO_PEERS);
}

bool NetworkManager::ConnectToServer()
{
	std::cout << serverIp << " " << serverPort << std::endl;

	if (serverSocket->connect(SERVER_IP, SERVER_PORT) == sf::Socket::Status::Done)
	{
		std::cout << "Server Connected" << std::endl;;
	}

	sf::Socket::Status status = serverSocket->connect(serverIp, serverPort);

	if (status == sf::Socket::Status::Done)
	{
		serverSocket->setBlocking(false);
		ChangeState(NetworkState::CONNECTED_TO_SERVER);
		std::cout << "Connected To server" << std::endl;

		StartListening();

		return true;
	}
	else if (status == sf::Socket::Status::NotReady)
	{
		if (socketSelector.wait())
		{
			if (socketSelector.isReady(*serverSocket))
			{
				ChangeState(NetworkState::CONNECTED_TO_SERVER);
				std::cout << "Connected To server" << std::endl;
			}
		}
	}

	std::cerr << "Can't connect to server because: " << static_cast<int>(status) << std::endl;
	return false;
}

void NetworkManager::DisconnectServer()
{
	if (serverSocket)
	{
		serverSocket->disconnect();
		serverSocket.reset();
	}

	ChangeState(NetworkState::DISCONNECTED);
	std::cout << "Disconnected from server" << std::endl;
}

void NetworkManager::DisconnectAllPeers()
{
	for (std::shared_ptr<Client> client : p2pClients)
		client->GetNetwork().GetSocket().disconnect();

	p2pClients.clear();

	ChangeState(NetworkState::DISCONNECTED);
	std::cout << "Disconnected from all peers" << std::endl;
}

void NetworkManager::RefreshSelector()
{
	selectorMutex.lock();
	socketSelector.clear();

	if (currentState == NetworkState::CONNECTED_TO_SERVER)
	{
		socketSelector.add(*serverSocket);
	}
	else if (currentState == NetworkState::CONNECTED_TO_PEERS)
	{
		for (std::shared_ptr<Client> client : p2pClients)
			socketSelector.add(client->GetNetwork().GetSocket());
	}
	selectorMutex.unlock();
}

NetworkState NetworkManager::GetNetworkState()
{
	NetworkState state;
	stateMutex.lock();
	state = currentState;
	stateMutex.unlock();

	return state;;
}
