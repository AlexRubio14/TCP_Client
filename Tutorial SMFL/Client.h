#include <string>
#include <SFML/Network.hpp>
#include "ClientState.h"

class Client
{
private:
    std::string guid;
    std::string username;
    std::unique_ptr<sf::TcpSocket> socket;
    ClientState clientState;

public:
    Client();
    ~Client();

    void HandleIncomingPackets();
    void InitializeClient(std::string _guid, std::string _username, std::unique_ptr<sf::TcpSocket> _socket);

    inline std::string GetGuid() const { return guid; }
    inline std::string GetUsername() const { return username; }
    inline sf::TcpSocket& GetSocket() { return *socket; }

    inline void SetGuid(const std::string _guid) { guid = _guid; }
    inline void SetUsername(const std::string& _username) { username = _username; }
    inline void SetState(const ClientState _clientState) { clientState = _clientState; }
};

