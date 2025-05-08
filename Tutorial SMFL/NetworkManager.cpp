#include "NetworkManager.h"
#include "PacketManager.h"
#include "GameManager.h"
#include <optional>



void NetworkManager::HandleServerCommunication()
{
	while (true)
	{
		stateMutex.lock();
		NetworkState state = currentState;
		stateMutex.unlock();

		if (state != NetworkState::CONNECTED_TO_SERVER)
			break;

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

	while (true)
	{
		{
			std::lock_guard<std::mutex> lock(stateMutex);
			if (currentState != NetworkState::CONNECTED_TO_PEERS)
				break;
		}

		if (socketSelector.wait(sf::seconds(0.1f)))
		{
			if (socketSelector.isReady(listener))
			{
				HandleNewConnections();
			}
			else
			{
				UpdateP2PClients();
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
	std::cout << static_cast<int>(currentState) << std::endl;
	stateMutex.unlock();

	switch (state) {
	case NetworkState::CONNECTED_TO_SERVER:
		HandleServerCommunication();
		break;
	case NetworkState::CONNECTED_TO_PEERS:
		std::cout << "Update p2p" << std::endl;
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
	std::cout << "state changed to :" << static_cast<int>(newState) << std::endl;
	currentState = newState;
	RefreshSelector();
	stateMutex.unlock();
}

void NetworkManager::StartListening()
{
	if (listener.listen(0) == sf::Socket::Status::Done)
	{
		std::cout << "Listening on port: " << listener.getLocalPort() << std::endl;
		std::cout << "Listening on port: " << GetListeningPort() << std::endl;
	}
	else
		std::cerr << "Failed to start Listening" << std::endl;
}

void NetworkManager::StartClientConnections(const std::vector<std::shared_ptr<Client>>& newClients, int myIndex, int port)
{
	p2pClients = newClients;

	std::optional<sf::IpAddress> localIp = sf::IpAddress::getLocalAddress();
	int localPort = NETWORK.GetListeningPort();

	for (int i = 0; i < p2pClients.size(); ++i)
	{
		if (i == myIndex) 
			continue;

		std::shared_ptr<Client>& newClient = p2pClients[i];
		NetworkClient& network = newClient->GetNetwork();
		std::optional<sf::IpAddress> ipAdress = sf::IpAddress::resolve(network.GetIp());

		std::cout << *ipAdress << std::endl;

		if (!ipAdress)
			continue;

		sf::Socket::Status status = network.GetSocket().connect(*ipAdress, network.GetPort());

		if (status == sf::Socket::Status::Done)
		{
			network.GetSocket().setBlocking(false);

			{
				std::lock_guard<std::mutex> lock(selectorMutex);
				socketSelector.add(network.GetSocket());
			}

			std::cout << "Connected to peer " << network.GetIp() << ":" << network.GetPort() << std::endl;
		}
		else
		{
			std::cerr << "Failed to connect to peer " << network.GetIp() << std::endl;
		}
	}

	std::cout << "Received: IP = " << p2pClients[0]->GetNetwork().GetIp() << " | Name = " << p2pClients[0]->GetPlayerData().GetUsername() << " | Index = " << p2pClients[0]->GetPlayerData().GetIndex() << " | Port = " << p2pClients[0]->GetNetwork().GetPort() << std::endl;
	std::cout << "Received: IP = " << p2pClients[1]->GetNetwork().GetIp() << " | Name = " << p2pClients[1]->GetPlayerData().GetUsername() << " | Index = " << p2pClients[1]->GetPlayerData().GetIndex() << " | Port = " << p2pClients[1]->GetNetwork().GetPort() << std::endl;

	GAME.StartGame();
	ChangeState(NetworkState::CONNECTED_TO_PEERS);
}

bool NetworkManager::ConnectToServer()
{
	std::cout << serverIp << " " << serverPort << std::endl;

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

void NetworkManager::HandleNewConnections()
{
	// Aceptar nuevas conexiones entrantes
	std::shared_ptr<sf::TcpSocket> newSocket = std::make_shared<sf::TcpSocket>();
	if (listener.accept(*newSocket) == sf::Socket::Status::Done)
	{
		std::optional<sf::IpAddress> remoteIp = newSocket->getRemoteAddress();
		unsigned short remotePort = newSocket->getRemotePort();

		std::cout << "New connection from: " << *remoteIp << ":" << remotePort << std::endl;

		bool linked = false;
		for (auto& client : p2pClients)
		{
			if (client == nullptr) continue;

			const std::string& expectedIp = client->GetNetwork().GetIp();
			unsigned short expectedPort = client->GetNetwork().GetPort();

			if (remoteIp->toString() == expectedIp && remotePort == expectedPort)
			{
				std::cout << " Link connection with existing client: " << expectedIp << ":" << expectedPort << std::endl;

				client->GetNetwork().SetSocket(newSocket);
				newSocket->setBlocking(false);

				{
					std::lock_guard<std::mutex> lock(selectorMutex);
					socketSelector.add(*newSocket);
				}

				linked = true;
				break;
			}
		}

		if (!linked)
		{
			std::cerr << " Can't link new connection with existing client " << std::endl;
			newSocket->disconnect();
		}
	}
}

void NetworkManager::UpdateP2PClients()
{
	for (std::shared_ptr<Client>& client : p2pClients)
	{
		if (!client) 
			continue;

		sf::TcpSocket& socket = client->GetNetwork().GetSocket();

		if (socket.getRemoteAddress() == sf::IpAddress::Any)
			continue;

		if (!socketSelector.isReady(socket))
			continue;

		CustomPacket customPacket;
		sf::Socket::Status status = socket.receive(customPacket.packet);

		switch (status)
		{
		case sf::Socket::Status::Done :
			PACKET_MANAGER.ProcessReceivedPacket(customPacket);
			break;

		case sf::Socket::Status::Disconnected:
		{
			std::cerr << "[P2P] Client disconnected: "
				<< *socket.getRemoteAddress() << ":" << socket.getRemotePort() << std::endl;

			std::lock_guard<std::mutex> lock(selectorMutex);
			socketSelector.remove(socket);
			// Clean client from vector
			break;
		}
		default:
			std::cerr << "Error receiving packet: " << static_cast<int>(status) << std::endl;
			break;
		}
	}
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

		socketSelector.add(listener);
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
