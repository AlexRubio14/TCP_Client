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
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Create room succes: " << responseMessage << std::endl;
		SCENE.ChangeScene(new GameScene());
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
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Join room succes: " << responseMessage << std::endl;

		SCENE.ChangeScene(new GameScene());
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM_ERROR, [this](CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Join Room error: " << responseMessage << std::endl;
		});

	EVENT_MANAGER.Subscribe(START_GAME, [this](CustomPacket& customPacket) {
		std::cout << "Start Game" << std::endl;

		int numPlayers = 2;

		std::string ip, name;
		int index, numPort, myIndex = -1;

		GAME.Init(SCENE.GetWindow());

		for (int i = 0; i < numPlayers; ++i)
		{
			customPacket.packet >> ip >> name >> index; 

			std::cout << "Received: IP = " << ip << " | Name = " << name << " | Index = " << index << std::endl;

			if (ip == sf::IpAddress::getLocalAddress()->toString())
				myIndex = index;

			numPort = NETWORK.GetListeningPort() + index;

			GAME.AddClient(ip, name, index, numPort);

			std::cout << "Client added: IP = " << ip << ", Port = " << numPort << std::endl;
		}

		if (myIndex == -1)
		{
			std::cout << "You haven't recognized yourself" << std::endl;
			return;
		}

		GAME.RecognizeClient(myIndex);
		EVENT_MANAGER.Emit(DISCONNECT, customPacket);
		NETWORK.StartClientConnections(GAME.GetClients(), GAME.GetReferenceClient()->GetPlayerData().GetIndex(), GAME.GetReferenceClient()->GetNetwork().GetPort());
		NETWORK.DisconnectServer();
		GAME.StartGame();
		
		});

	EVENT_MANAGER.Subscribe(END_TURN, [this](CustomPacket& customPacket) {
		std::cout << "End Turn" << std::endl;

		for (int i = 0; i < GAME.GetClients().size(); i++)
		{
			if (GAME.GetReferenceClient()->GetPlayerData().GetIndex() == GAME.GetClients()[i]->GetPlayerData().GetIndex())
				continue;

			CustomPacket responsePacket(END_TURN_SUCCES);
			responsePacket.packet << "Turn ended";
		
			SendPacketToClient(GAME.GetClients()[i], responsePacket);
		}
		});

	EVENT_MANAGER.Subscribe(END_TURN_SUCCES, [this](CustomPacket& customPacket) {
		
		std::string responseMessage;
		std::cout << "End turn succes" << std::endl;
		customPacket.packet >> responseMessage;
		std::cout << responseMessage;
		GAME.EndTurn();
		});

	EVENT_MANAGER.Subscribe(DISCONNECT, [this](CustomPacket& customPacket) {
		std::cout << "Server disconnect" << std::endl;
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
	std::cout << responsePacket.type << std::endl;
	if (client->GetNetwork().GetSocket().send(responsePacket.packet) == sf::Socket::Status::Done)
		std::cout << "Message sent to client: "<<client->GetNetwork().GetIp()<< " "<<client->GetPlayerData().GetUsername()<<" "<<client->GetNetwork().GetPort() << std::endl;
	else
		std::cerr << "Error sending the message to client" << std::endl;
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

