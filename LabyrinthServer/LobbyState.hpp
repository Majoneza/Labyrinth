#pragma once
#ifndef LOBBYSTATE_H
#define LOBBYSTATE_H

#include <SFML/Network.hpp>
#include "GameState.hpp"

class Client;
class LobbyState : public GameState
{
public:
	LobbyState(GameStateManager& gsm, const sf::Uint16& port, const sf::IpAddress& address);

	~LobbyState();

	void onTick() override;
private:
	void startGame();

	sf::TcpListener listener;

	bool waitingForPlayers;

	sf::Clock timer;
};

#endif