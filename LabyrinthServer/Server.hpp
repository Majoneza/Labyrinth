#pragma once
#ifndef SERVER_H
#define SERVER_H

#include "GameStateManager.hpp"
#include "dynarray.hpp"
#include <vector>
#include <thread>
#include "Map.hpp"
#include "Client.hpp"

class Tile;
class Server : sf::NonCopyable
{
public:
	//Server request types
	enum RequestType { VerifyRequest, ReadyRequest, TilesRequest, MapRequest, PositionRequest, LobbyRequest, ResultRequest, TimeRequest, PlayersRequest, DisconnectRequest, RequestAmount };

	//Server send type
	enum ReplyType { VerifiedReply, PasswordRequiredReply, UnverifiedReply, ReadyReply, TilesReply, MapReply, PositionReply, LobbyReply, ResultReply, TimeReply, PlayersReply, GameOverReply, DisconnectReply, ShutdownReply, StartReply, ReplyAmount };

	//Server construct
	Server(const sf::Vector2u& mapSize, const float& difficulty, const sf::Uint8& min_client_number, const sf::Uint8& max_client_number, const sf::Uint16& player_wait, const sf::Uint16& port, const sf::IpAddress& address = sf::IpAddress::Any);

	//Server clients
	dynarray<Client> clients;

	//Server minimum client number to start the game
	const sf::Uint8 minClientNumber;

	//Server time to wait for players to connect
	const sf::Uint16 playerWait;

	//Server address
	const sf::IpAddress address;

	//Server port
	const sf::Uint16 port;

	//Server clients lock
	mutable sf::Mutex clientsLock;

	//Server map size
	const sf::Vector2u mapSize;

	//Server map difficulty
	const float difficulty;

	//Server maps
	std::vector<std::unique_ptr<Map>> maps;

	//Server maps lock
	sf::Mutex mapsLock;

	//Server banned ips
	std::vector<sf::IpAddress> bannedIps;

	//Server banned ips lock
	sf::Mutex bannedIpsLock;

	//Server password
	std::string password;

	//Server password lock
	sf::Mutex passwordLock;

	//Server get connected client number
	sf::Uint32 getConnectedClientNumber() const;

	//Server create map
	void createMap();

	//Server contains name
	bool nameExists(const sf::String& name) const;

	//Server contains name
	bool nameExists(const sf::String& name, Client* exclude) const;

	//Server get client by name
	bool getClientByName(Client*& client, const std::string& name);
private:
	//Server command line
	std::thread commandLine;

	//Server command function
	void handleCommands();

	//Server main function
	void run();

	//Server shut down
	void shutdown();

	//Server game state manager
	GameStateManager gsm;

	//Server running state
	std::atomic_bool running;
};

sf::Packet& operator>>(sf::Packet& packet, Server::RequestType& m);

sf::Packet& operator>>(sf::Packet& packet, Server::ReplyType& m);

sf::Packet& operator<<(sf::Packet& packet, const Tile& tile);

sf::Packet& operator>>(sf::Packet& packet, Tile& tile);

template<typename T>
sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2<T>& m)
{
	return packet << m.x << m.y;
}

template<typename T>
sf::Packet& operator>>(sf::Packet& packet, sf::Vector2<T>& m)
{
	return packet >> m.x >> m.y;
}

#endif