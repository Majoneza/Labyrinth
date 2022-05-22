#include "stdafx.h"
#include "PlayState.hpp"
#include "Server.hpp"
#include "GameOverState.hpp"

PlayState::PlayState(GameStateManager& gsm) :
	GameState(gsm)
{
	printf("Game started\n");
}

void PlayState::onTick()
{
	if (isGameOver())
	{
		printf("Game over\n");
		sf::Packet packet;
		packet << Server::ReplyType::GameOverReply;
		{
			sf::Lock lock(server.clientsLock);
			for (Client& client : server.clients)
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
			}
		}
		gsm.setState(new GameOverState(gsm));
	}
}

bool PlayState::isGameOver() const
{
	sf::Lock lock(server.clientsLock);
	for (const Client& client : server.clients)
	{
		if (client.verified && client.player.timeLeft.get() != 0.f) return false;
	}
	return true;
}