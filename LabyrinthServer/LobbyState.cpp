#include "stdafx.h"
#include "LobbyState.hpp"
#include "PlayState.hpp"
#include "Client.hpp"
#include "Server.hpp"

LobbyState::LobbyState(GameStateManager& gsm, const sf::Uint16& port, const sf::IpAddress& address) :
	GameState(gsm),
	waitingForPlayers(false)
{
	listener.setBlocking(false);
	if (listener.listen(port, address) != sf::Socket::Done) printf("Listen failed\n");
}

LobbyState::~LobbyState()
{
	listener.close();
}

void LobbyState::onTick()
{
	{
		sf::Lock lock(server.clientsLock);
		for (Client& client : server.clients)
		{
			if (client.running == false)
			{
				if (listener.accept(client.tcp) == sf::Socket::Done)
				{
					sf::Lock lk(server.bannedIpsLock);
					if (std::find(server.bannedIps.begin(), server.bannedIps.end(), client.tcp.getRemoteAddress()) == server.bannedIps.end())
					{
						client.start();
					}
					else client.tcp.disconnect();
				}
			}
		}
	}
	const auto readyPlayers = std::count_if(server.clients.cbegin(), server.clients.cend(), [](const Client& client) -> bool { return client.ready; });
	if (readyPlayers == server.clients.size())
		startGame();
	else if (readyPlayers >= server.minClientNumber && waitingForPlayers == false)
	{
		printf("Waiting for remaining players for %d seconds.\n", server.playerWait);
		waitingForPlayers = true;
		timer.restart();
	}
	else if (waitingForPlayers == true && timer.getElapsedTime().asSeconds() >= server.playerWait)
	{
		printf("Starting game with %d players.\n", readyPlayers);
		startGame();
	}
	else if (waitingForPlayers == true && readyPlayers < server.minClientNumber)
	{
		printf("Waiting canceled.\n");
		waitingForPlayers = false;
	}
}

void LobbyState::startGame()
{
	listener.close();
	printf("Broadcasting\n");
	sf::Packet packet;
	packet << Server::ReplyType::StartReply;
	{
		sf::Lock lock(server.clientsLock);
		for (Client& client : server.clients)
		{
			if (client.verified)
			{
				switch (client.tcp.send(packet))
				{
				case sf::Socket::Status::Done:
					printf("%s: broadcast successful\n", client.getName().c_str());
					break;
				case sf::Socket::Status::Disconnected:
					printf("%s: broadcast failed(disconnected)\n", client.getName().c_str());
					break;
				case sf::Socket::Status::Error:
					printf("%s: broadcast failed(error)\n", client.getName().c_str());
					break;
				case sf::Socket::Status::NotReady:
					printf("%s: broadcast failed(not ready)\n", client.getName().c_str());
					break;
				case sf::Socket::Status::Partial:
					printf("%s: broadcast failed(partial)\n", client.getName().c_str());
					break;
				}
				client.player.timeLeft.reset();
				client.player.timeLeft.set((server.mapSize.x * server.mapSize.y) / server.difficulty);
			}
			else client.disconnect();
		}
	}
	gsm.setState(new PlayState(gsm));
}