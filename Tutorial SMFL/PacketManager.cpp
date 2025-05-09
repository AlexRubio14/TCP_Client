#include "PacketManager.h"
#include <iostream>
#include "EventManager.h"
#include "NetworkManager.h"
#include "SceneManager.h"
#include "GameManager.h"

void PacketManager::HandleHandshake(sf::Packet& packet)
{
	std::string messageFromServer;
	packet >> messageFromServer;

	std::cout << "Messages received from server: " << messageFromServer << std::endl;
}

void PacketManager::HandleTest(sf::Packet& packet)
{
	std::string message;
	packet >> message;

	std::cout << "Message received from server: " << message << std::endl;
}

void PacketManager::SendHandshakeP2P(const std::shared_ptr<Client>& client)
{
	CustomPacket customPacket(HANDSHAKE_P2P);
	std::string responseMessage = "Hello, im the new client";

	std::string myGuid = GAME.GetReferenceClient()->GetNetwork().GetGuid();
	int myPort = NETWORK.GetListeningPort();
	customPacket.packet << myGuid << myPort << responseMessage;

	SendPacketToClient(client, customPacket);
}

void PacketManager::SendHandshake(const std::string guid)
{
	CustomPacket customPacket(HANDSHAKE);
	customPacket.packet << "Hello Server, I'm the new client";

	SendPacketToServer(customPacket);
}

void PacketManager::Init()
{
	EVENT_MANAGER.Subscribe(HANDSHAKE, [this](CustomPacket& customPacket) {
		HandleHandshake(customPacket.packet);
		});

	EVENT_MANAGER.Subscribe(REGISTER, [this](CustomPacket& customPacket) {
		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		SendPacketToServer(customPacket);

		});

	EVENT_MANAGER.Subscribe(REGISTER_ERROR, [this](CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;
		
		std::cout << "Register error: " << responseMessage << std::endl;
		});

	EVENT_MANAGER.Subscribe(REGISTER_SUCCES, [this](CustomPacket& customPacket) {
		std::cout << "Register succes" << std::endl;
		SCENE.ChangeScene(new LobbyScene());
		});

	EVENT_MANAGER.Subscribe(LOGIN, [this](CustomPacket& customPacket) {

		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		SendPacketToServer(customPacket);
		});

	EVENT_MANAGER.Subscribe(LOGIN_ERROR, [this](CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Login error: " << responseMessage << std::endl;

		});

	EVENT_MANAGER.Subscribe(LOGIN_SUCCESS, [this](CustomPacket& customPacket) {
		std::cout << "Login succes" << std::endl;
		SCENE.ChangeScene(new LobbyScene());
		});

	EVENT_MANAGER.Subscribe(CREATE_ROOM, [this](CustomPacket& customPacket) {
		std::string id;
		customPacket.packet >> id;

		SendPacketToServer(customPacket);

		});

	EVENT_MANAGER.Subscribe(CREATE_ROOM_SUCCES, [this](CustomPacket& customPacket) {
		std::string responseMessageCreateRoomSucces;

		customPacket.packet >> responseMessageCreateRoomSucces;

		std::cout << "Create room succes: " << responseMessageCreateRoomSucces << std::endl;
		SCENE.ChangeScene(new GameScene());

		CustomPacket responsePacket(ENTER_ROOM);
		std::string responseMessageEnterRoom = std::to_string(NETWORK.GetListeningPort());

		std::cout << "Send packet to client with the port im listening: " << responseMessageEnterRoom << std::endl;
		responsePacket.packet << responseMessageEnterRoom;
		EVENT_MANAGER.Emit(ENTER_ROOM, responsePacket);
		});

	EVENT_MANAGER.Subscribe(CREATE_ROOM_ERROR, [this](CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Create Room error: " << responseMessage << std::endl;
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM, [this](CustomPacket& customPacket) {
		std::string id;
		customPacket.packet >> id;

		SendPacketToServer(customPacket);
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM_SUCCES, [this](CustomPacket& customPacket) {
		std::string responseMessageJoinRoomSucces;

		customPacket.packet >> responseMessageJoinRoomSucces;

		std::cout << "Join room succes: " << responseMessageJoinRoomSucces << std::endl;

		SCENE.ChangeScene(new GameScene());

		CustomPacket responsePacket(ENTER_ROOM);
		std::string responseMessageEnterRoom = std::to_string(NETWORK.GetListeningPort());

		responsePacket.packet << responseMessageEnterRoom;
		std::cout << "Packet send to client with the port im listening: " << responseMessageEnterRoom << std::endl;
		EVENT_MANAGER.Emit(ENTER_ROOM, responsePacket);
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM_ERROR, [this](CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Join Room error: " << responseMessage << std::endl;
		});

	EVENT_MANAGER.Subscribe(ENTER_ROOM, [this](CustomPacket& customPacket) {
		SendPacketToServer(customPacket);
		std::cout << "Waiting for more players..." << std::endl;
		});

	EVENT_MANAGER.Subscribe(START_GAME, [this](CustomPacket& customPacket) {
		std::cout << "Start Game" << std::endl;

		int numPlayers = 2;

		std::string ip, name, guid;
		int index, myIndex, numPort = -1;

		GAME.Init(SCENE.GetWindow());

		std::optional<sf::IpAddress> localIp = sf::IpAddress::getLocalAddress();
		int localPort = NETWORK.GetListeningPort();

		customPacket.packet >> myIndex;

		for (int i = 0; i < numPlayers; ++i)
		{
			customPacket.packet >> ip >> name >> index >> numPort >> guid; 

			std::cout << "Received: IP = " << ip << " | Name = " << name << " | Index = " << index << " | Port = " << numPort << " | Guid " << guid << std::endl;

			GAME.AddClient(ip, name, index, numPort, guid);

			std::cout << "Client added: IP = " << ip << ", Port = " << numPort << std::endl;
		}

		if (myIndex == -1)
		{
			std::cout << "You haven't recognized yourself" << std::endl;
			return;
		}

		GAME.RecognizeClient(myIndex);
		EVENT_MANAGER.Emit(DISCONNECT, customPacket); // Request the server to delete my data
		NETWORK.DisconnectServer();
		NETWORK.StartClientConnections(GAME.GetClients(), GAME.GetReferenceClient()->GetPlayerData().GetIndex(), GAME.GetReferenceClient()->GetNetwork().GetPort());
		GAME.StartGame();

		});

	EVENT_MANAGER.Subscribe(END_TURN, [this](CustomPacket& customPacket) {
		std::cout << "End Turn" << std::endl;

		for (int i = 0; i < NETWORK.GetClients().size(); i++)
		{
			if (GAME.GetReferenceClient()->GetPlayerData().GetIndex() == NETWORK.GetClients()[i]->GetPlayerData().GetIndex())
				continue;

			CustomPacket responsePacket(END_TURN_SUCCES);
			responsePacket.packet << "Turn ended";
		
			SendPacketToClient(NETWORK.GetClients()[i], responsePacket);
		}
		});

	EVENT_MANAGER.Subscribe(END_TURN_SUCCES, [this](CustomPacket& customPacket) {
		std::cout << "Packet Received from other client" << std::endl;
		std::string responseMessage;
		std::cout << "End turn succes" << std::endl;
		customPacket.packet >> responseMessage;
		std::cout << responseMessage;
		GAME.EndTurn(true);
		});

	EVENT_MANAGER.Subscribe(DISCONNECT, [this](CustomPacket& customPacket) {

		NetworkState networkState = NETWORK.GetNetworkState();
		std::string responseMessage;

		if (networkState == NetworkState::CONNECTED_TO_SERVER)
		{
			responseMessage = "The client has closed the game";
			customPacket.packet >> responseMessage;
			SendPacketToServer(customPacket);
		}
		else if (networkState == NetworkState::CONNECTED_TO_PEERS)
		{
			CustomPacket responsePacket(PEER_DISCONNECTED);
			responseMessage = GAME.GetReferenceClient()->GetPlayerData().GetIndex();
			responsePacket.packet >> responseMessage;

			for (std::shared_ptr<Client> client : NETWORK.GetClients())
			{
				SendPacketToClient(client, responsePacket);
			}
		}
		});

	EVENT_MANAGER.Subscribe(HANDSHAKE_P2P, [this](CustomPacket& customPacket) {

		std::string guid;
		int port;
		std::string message;
		customPacket.packet >> guid >> port >> message;

		if (guid == GAME.GetReferenceClient()->GetNetwork().GetGuid())
			return;

		std::shared_ptr<Client> client = NETWORK.GetClientByGuid(guid);

		if (client == nullptr)
		{
			std::cerr << "HandshakeP2P: Unknown client guid " << guid << port << std::endl;
			return;
		}

		std::cout << "Received handshake from " << client->GetPlayerData().GetUsername()<< "and port: " <<  port << " : " << message << std::endl;

		
		client->GetNetwork().SetPort(port);
		std::cout << "Updated client port: " << port << std::endl;

		});
}

void PacketManager::ProcessReceivedPacket(CustomPacket& customPacket)
{
	customPacket.packet >> customPacket.type;
	std::cout << customPacket.type << std::endl;
	EVENT_MANAGER.Emit(customPacket.type, customPacket);
}

void PacketManager::SendPacketToClient(const std::shared_ptr<Client> client, CustomPacket& responsePacket)
{
	sf::Socket::Status status = client->GetNetwork().GetSocket().send(responsePacket.packet);
	if (status == sf::Socket::Status::Done)
		std::cout << "Message sent to client: " <<client->GetNetwork().GetIp()<< " "<<client->GetPlayerData().GetUsername()<<" "<<client->GetNetwork().GetPort() << std::endl;
	else
	{
		std::cout << "The error is: " << static_cast<int>(status) << std::endl;
		std::cerr << "Error sending the message to client" << client->GetNetwork().GetIp() << " " << client->GetPlayerData().GetUsername() << " " << client->GetNetwork().GetPort() << std::endl;
	}
}

void PacketManager::SendPacketToServer(CustomPacket& customPacket)
{
	if (NETWORK.GetServerSocket()->send(customPacket.packet) == sf::Socket::Status::Done)
	{
		std::cout << "Packet send to server" << std::endl;
	}
	else
		std::cout << "Error sending packet to server" << std::endl;
}

