#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <SFML\Network.hpp>

class Tile;
class Client
{
public:
	//Client request types
	enum RequestType { VerifyRequest, ReadyRequest, TilesRequest, MapRequest, PositionRequest, LobbyRequest, ResultRequest, TimeRequest, PlayersRequest, DisconnectRequest, RequestAmount };

	//Client send type
	enum ReplyType { VerifiedReply, PasswordRequiredReply, UnverifiedReply, ReadyReply, TilesReply, MapReply, PositionReply, LobbyReply, ResultReply, TimeReply, PlayersReply, GameOverReply, DisconnectReply, ShutdownReply, StartReply, ReplyAmount };

	//Client constructor
	Client();

	//Client destructor
	~Client();

	//Client is connected on tcp
	bool isTcpConnected() const;

	//Client disconnect from server
	void disconnect();

	//Client send packet over udp socket
	sf::Socket::Status udpSend(const sf::IpAddress& address, const unsigned short& port, sf::Packet& packet);

	//Client id
	sf::Uint16 id;

	//Client name
	sf::String name;

	//Client server address
	sf::IpAddress serverAddress;

	//Client server port
	unsigned short serverPort;

	//Client TCP socket
	sf::TcpSocket tcp;

	//Client UDP socket
	sf::UdpSocket udp;
};

//Client function
sf::Packet& operator >>(sf::Packet& packet, Client::ReplyType& m);

//Client function
sf::Packet& operator >>(sf::Packet& packet, Client::RequestType& m);

//Client function
sf::Packet& operator<<(sf::Packet& packet, const Tile& tile);

//Client function
sf::Packet& operator>>(sf::Packet& packet, Tile& tile);

//Client function
template<typename T>
sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2<T>& m)
{
	return packet << m.x << m.y;
}

//Client function
template<typename T>
sf::Packet& operator>>(sf::Packet& packet, sf::Vector2<T>& m)
{
	return packet >> m.x >> m.y;
}
#endif