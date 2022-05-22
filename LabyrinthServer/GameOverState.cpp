#include "stdafx.h"
#include "GameOverState.hpp"
#include "Server.hpp"
#include "LobbyState.hpp"

GameOverState::GameOverState(GameStateManager& gsm) :
	GameState(gsm)
{
}

void GameOverState::onTick()
{
	//sf::Lock lock(server.clientsLock);
	//if (std::count_if(server.clients.cbegin(), server.clients.cend(), [](const Client& client) -> bool { return client.running; }) == 0) reset();
}

void GameOverState::reset() const
{
	printf("Resetting server...\n");
	{
		sf::Lock lock(server.mapsLock);
		server.maps.clear();
	}
	server.createMap();
	{
		sf::Lock lock(server.clientsLock);
		for (Client& client : server.clients)
			client.player.mapNumber = 0;
	}
	gsm.setState(new LobbyState(gsm, server.port, server.address));
	printf("Ready\n");
}