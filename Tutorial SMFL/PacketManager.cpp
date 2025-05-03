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
	EVENT_MANAGER.Subscribe(HANDSHAKE, [this](std::string guid, CustomPacket& customPacket) {
		HandleHandshake(customPacket.packet);
		});

	EVENT_MANAGER.Subscribe(REGISTER, [this](std::string guid, CustomPacket& customPacket) {
		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		SendPacketToServer(customPacket);

		});

	EVENT_MANAGER.Subscribe(REGISTER_ERROR, [this](std::string guid, CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;
		
		std::cout << "Register error: " << responseMessage << std::endl;
		});

	EVENT_MANAGER.Subscribe(REGISTER_SUCCES, [this](std::string guid, CustomPacket& customPacket) {
		std::cout << "Register succes" << std::endl;
		SCENE.ChangeScene(new LobbyScene());
		});

	EVENT_MANAGER.Subscribe(LOGIN, [this](std::string guid, CustomPacket& customPacket) {

		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		SendPacketToServer(customPacket);
		});

	EVENT_MANAGER.Subscribe(LOGIN_ERROR, [this](std::string guid, CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Login error: " << responseMessage << std::endl;

		});

	EVENT_MANAGER.Subscribe(LOGIN_SUCCESS, [this](std::string guid, CustomPacket& customPacket) {
		std::cout << "Login succes" << std::endl;
		SCENE.ChangeScene(new LobbyScene());
		});

	EVENT_MANAGER.Subscribe(CREATE_ROOM, [this](std::string guid, CustomPacket& customPacket) {
		std::string id;
		customPacket.packet >> id;

		SendPacketToServer(customPacket);

		});

	EVENT_MANAGER.Subscribe(CREATE_ROOM_SUCCES, [this](std::string guid, CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Create room succes: " << responseMessage << std::endl;
		SCENE.ChangeScene(new GameScene());
		});

	EVENT_MANAGER.Subscribe(CREATE_ROOM_ERROR, [this](std::string guid, CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Create Room error: " << responseMessage << std::endl;
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM, [this](std::string guid, CustomPacket& customPacket) {
		std::string id;
		customPacket.packet >> id;

		SendPacketToServer(customPacket);
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM_SUCCES, [this](std::string guid, CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Join room succes: " << responseMessage << std::endl;

		SCENE.ChangeScene(new GameScene());
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM_ERROR, [this](std::string guid, CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Join Room error: " << responseMessage << std::endl;
		});

	EVENT_MANAGER.Subscribe(START_GAME, [this](std::string guid, CustomPacket& customPacket) {
		std::cout << "Start Game" << std::endl;

		std::string ip;
		std::string name;
		int index;

		GAME.Init(SCENE.GetWindow());
		for (int i = 0; i < 4; i++)
		{
			customPacket.packet >> ip >> name >> index;
			GAME.AddClient(ip, name, index);
		}
		});
}

void PacketManager::ProcessPacket(std::string guid, CustomPacket& customPacket)
{
	customPacket.packet >> customPacket.type;

	EVENT_MANAGER.Emit(customPacket.type, guid, customPacket);
}

void PacketManager::SendPacketToClient(const std::shared_ptr<Client> client, CustomPacket& responsePacket)
{
	if (client->GetSocket().send(responsePacket.packet) == sf::Socket::Status::Done)
		std::cout << "Message sent to client " << std::endl;
	else
		std::cerr << "Error sending the message to client" << std::endl;
}

void PacketManager::SendPacketToServer(CustomPacket& customPacket)
{
	if (NETWORK.GetSocketServer().send(customPacket.packet) == sf::Socket::Status::Done)
	{
		std::cout << "Packet send to server" << std::endl;
	}
	else
		std::cout << "Error sending packet to server" << std::endl;
}

