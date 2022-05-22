#include "stdafx.h"
#include "Server.hpp"
#include "LobbyState.hpp"
#include "Tile.hpp"
#include <iostream>

Server::Server(const sf::Vector2u& mapSize, const float& difficulty, const sf::Uint8& min_client_number, 
	const sf::Uint8& max_client_number, const sf::Uint16& player_wait, const sf::Uint16& port, const sf::IpAddress& address) :
	clients(max_client_number),
	minClientNumber(min_client_number),
	playerWait(player_wait),
	address(address),
	port(port),
	mapSize(mapSize),
	difficulty(difficulty),
	commandLine(&Server::handleCommands, this),
	gsm(*this),
	running(true)
{
	createMap();
	{
		sf::Lock lock(clientsLock);
		for (Client& client : clients)
		{
			client.server = this;
			client.player.setMap(maps[client.player.mapNumber].get());
		}
	}
	printf("Ready\n");
	gsm.setState(new LobbyState(gsm, port, address));
	run();
}

void Server::run()
{
	const std::chrono::milliseconds ms(1);
	
	while (running)
	{
		gsm.onTick();

		std::this_thread::sleep_for(ms);
	}
	{
		sf::Lock lock(clientsLock);
		for (Client& client : clients)
			client.shutdown();
	}
	if (commandLine.joinable())
		commandLine.join();
}

std::vector<std::string> splitString(const std::string& s, const char& c)
{
	std::string buffer("");
	std::vector<std::string> v;
	for (const char& ch : s)
	{
		if (ch != c) buffer += ch;
		else if (ch == c && buffer != "") { v.push_back(buffer); buffer = ""; }
	}
	if (buffer != "") v.push_back(buffer);
	return v;
}

void Server::handleCommands()
{
	std::string input;
	while (running)
	{
		if (!std::getline(std::cin, input)) handleCommands();
		if (!input.empty())
		{
			auto args = splitString(input, ' ');
			printf(" ");
			if (args[0] == "players")
			{
				printf("Players:");
				{
					sf::Lock lock(clientsLock);
					for (size_t i = 0; i < clients.size(); i++)
					{
						if (clients[i].verified) printf(" |%s(%s)| ", clients[i].getName().c_str(), clients[i].tcp.getRemoteAddress().toString().c_str());
					}
				}
				printf("\n");
			}
			else if (args[0] == "kick" && args.size() == 2)
			{
				Client* client = nullptr;
				if (getClientByName(client, args[1]))
				{
					client->disconnect();
					printf("Player kicked!\n");
				}
				else printf("Player not found!\n");
			}
			else if (args[0] == "ban" && args.size() == 3)
			{
				if (args[1] == "ip")
				{
					printf("IP banned!\n");
					sf::Lock lock(bannedIpsLock);
					bannedIps.push_back(args[2]);
				}
				else if (args[1] == "player")
				{
					Client* client = nullptr;
					if (getClientByName(client, args[2]))
					{
						printf("Player banned!\n");
						{
							sf::Lock lock(bannedIpsLock);
							bannedIps.push_back(client->tcp.getRemoteAddress());
						}
						client->disconnect();
					}
					else printf("Player not found!\n");
				}
				else printf("Unknown command!\n");
			}
			else if (args[0] == "unban" && args.size() >= 2)
			{
				if (args[1] == "ip" && args.size() == 3)
				{
					sf::Lock lock(bannedIpsLock);
					auto it = std::find(bannedIps.cbegin(), bannedIps.cend(), args[2]);
					if (it != bannedIps.cend())
					{
						bannedIps.erase(it);
						printf("IP unbanned\n");
					}
					else printf("IP not found!\n");
				}
				else if (args[1] == "all")
				{
					printf("All players unbanned\n");
					sf::Lock lock(bannedIpsLock);
					bannedIps.clear();
				}
				else printf("Unknown command!\n");
			}
			else if (args[0] == "set" && args.size() == 3)
			{
				if (args[1] == "password")
				{
					printf("Password set!\n");
					sf::Lock lock(passwordLock);
					password = args[2];
				}
			}
			else if (args[0] == "remove" && args.size() == 2)
			{
				if (args[1] == "password")
				{
					printf("Password removed!\n");
					sf::Lock lock(passwordLock);
					password = "";
				}
			}
			//else if (args[0] == "reset") gsm.setState(new LobbyState(gsm, port, ip));
			else if (args[0] == "shutdown")
			{
				printf("Shutting down...\n");
				shutdown();
			}
			else printf("Unknown command!\n");
		}
	}
}

void Server::shutdown()
{
	running = false;
}

sf::Uint32 Server::getConnectedClientNumber() const
{
	sf::Lock lock(clientsLock);
	return std::count_if(clients.cbegin(), clients.cend(), [](const Client& client) -> bool { return client.isTcpConnected(); });
}

void Server::createMap()
{
	sf::Lock lock(mapsLock);
	maps.push_back(std::make_unique<Map>(mapSize));
	maps.back()->createMap();
}

bool Server::nameExists(const sf::String& name) const
{
	sf::Lock lock(clientsLock);
	for (const Client& client : clients)
	{
		if (client.getName() == name) return true;
	}
	return false;
}

bool Server::nameExists(const sf::String& name, Client* exclude) const
{
	sf::Lock lock(clientsLock);
	for (auto it = clients.cbegin(); it != clients.cend(); ++it)
	{
		if (it->getName() == name && it != exclude) return true;
	}
	return false;
}

bool Server::getClientByName(Client*& client, const std::string& name)
{
	sf::Lock lock(clientsLock);
	for (Client& c : clients)
	{
		if (c.getName() == name)
		{
			client = &c;
			return true;
		}
	}
	return false;
}

sf::Packet& operator>>(sf::Packet& packet, Server::RequestType& m)
{
	unsigned int i;
	packet = packet >> i;
	m = static_cast<Server::RequestType>(i);
	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Server::ReplyType& m)
{
	unsigned int i;
	packet = packet >> i;
	m = static_cast<Server::ReplyType>(i);
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