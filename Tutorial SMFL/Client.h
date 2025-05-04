#pragma once
#include <string>
#include <SFML/Network.hpp>
#include "ClientState.h"
#include "Token.h"

class Client
{
private:
    std::string ip;
    const int index;
    const std::string username;
    std::unique_ptr<sf::TcpSocket> socket;

    std::vector<std::shared_ptr<Token>> tokens;
    std::string name;
    sf::Color color;
    bool canThrowDice;
    int diceValue;
    bool extraMoves;

public:
    Client(const std::string& _ip, const std::string &_name, const sf::Color &_color, const int &_index);

    void HandleIncomingPackets();
    //void InitializeClient(std::string _guid, std::string _username, std::unique_ptr<sf::TcpSocket> _socket);

    inline std::string GetUsername() const { return username; }
    inline sf::TcpSocket& GetSocket() { return *socket; }

    int ThrowDice();
    void ControlDice();
    void SelectToken(sf::Vector2f mousePosition);
    bool ControlInteraction(const std::shared_ptr<Token>& token);
    void ControlNextTurn(const std::shared_ptr<Token>& token);
    void Update(sf::RenderWindow& window);

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window);
    std::string GetColorString();

    bool AllTokensInBase();
    bool AnyTokenInBase();
    bool HasTokensInSameCell() const;

    inline std::vector<std::shared_ptr<Token>> GetTokens() const { return tokens; }
    inline const std::string GetName() const { return name; }
    inline const int GetIndex() const { return index; }
    inline std::string GetIp() const { return ip; }

    inline void SetCanThrowDice(const bool state) { canThrowDice = state; }
    inline void SetExtraMoves(const bool state) { extraMoves = state; }

    inline void ResetDiceValue() { diceValue = 0; }
};

