#include "stdafx.h"
#include "Client.hpp"
#include "Tile.hpp"

Client::Client() :
	id(),
	serverAddress(sf::IpAddress::None),
	serverPort(0)
{
	tcp.setBlocking(true);
	udp.setBlocking(false);
}

Client::~Client()
{
	udp.unbind();
}

bool Client::isTcpConnected() const
{
	return tcp.getRemoteAddress() == sf::IpAddress::None || tcp.getRemotePort() == 0 ? false : true;
}

void Client::disconnect()
{
	serverAddress = sf::IpAddress::None;
	serverPort = 0;
	tcp.disconnect();
	udp.unbind();
}

sf::Socket::Status Client::udpSend(const sf::IpAddress& address, const unsigned short& port, sf::Packet& packet)
{
	if (packet.getDataSize() >= sf::UdpSocket::MaxDatagramSize) printf("OVER DATAGRAM SIZE\n");
	return udp.send(packet, address, port);
}

sf::Packet& operator >>(sf::Packet& packet, Client::ReplyType& m)
{
	unsigned int i;
	packet = packet >> i;
	m = static_cast<Client::ReplyType>(i);
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Client::RequestType& m)
{
	unsigned int i;
	packet = packet >> i;
	m = static_cast<Client::RequestType>(i);
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const Tile& tile)
{
	return packet << static_cast<sf::Int32>(tile.type) << tile.getPosition();
}

sf::Packet& operator>>(sf::Packet& packet, Tile& tile)
{
	sf::Int32 type;
	sf::Vector2u position;
	packet = packet >> type >> position;
	tile.type = static_cast<Tile::Type>(type);
	tile.setPosition(position);
	return packet;
}