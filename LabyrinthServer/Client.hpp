#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <SFML\Network.hpp>
#include "Player.hpp"
#include <thread>
#include <atomic>

class Server;
class Client : public sf::NonCopyable
{
public:
	//Client unknown name
	static const sf::String Unknown;

	//Client constructor
	Client();

	//Client start
	void start();

	//Client stop
	void stop();

	//Client shutdown
	void shutdown();

	//Client disconnect
	void disconnect();

	//Client is connected on tcp
	bool isTcpConnected() const;

	//bool Client::isUdpConnected() const

	//Client send over udp
	sf::Socket::Status udpSend(const sf::IpAddress& address, const unsigned short& port, sf::Packet& packet);

	//Client player
	Player player;

	//Client ready state
	std::atomic_bool ready;

	//Client server
	Server* server;

	//Client running state
	std::atomic_bool running;

	//Client verified state
	std::atomic_bool verified;

	//Client address
	sf::IpAddress address;

	//Client port
	unsigned short port;

	//Client tcp socket
	sf::TcpSocket tcp;

	//Client udp socket
	sf::UdpSocket udp;

	//Client name lock
	mutable sf::Mutex nameLock;

	//Client get name
	std::string getName() const;
private:
	//Client run
	void run();

	//Client thread
	std::thread thread;

	//Client generate id
	void generateID();

	//Client handle tcp request
	void handleTcpRequest();

	//Client handle udp request
	void handleUdpRequest();

	//Client name
	sf::String name;

	//Client id
	sf::Uint16 id;

	//Client request packet
	sf::Packet requestPacket;

	//Client reply packet
	sf::Packet replyPacket;
};

#endif