#pragma once
#ifndef LOBBYSTATE_H
#define LOBBYSTATE_H

#include "GameState.hpp"
#include <SFML\Network.hpp>
#include "Button.hpp"
#include "TextBox.hpp"

class LobbyState : public GameState
{
public:
	//LobbyState constructor
	LobbyState(GameStateManager& gsm, const sf::Uint8& client_number);

	//LobbyState destructor
	~LobbyState() = default;

	//LobbyState onEvent handler
	void onEvent(const sf::Event& evn) override;

	//LobbyState onTick handler
	void onTick() override;

	//LobbyState onRender handler
	void onRender(sf::RenderWindow& win) override;
private:
	//LobbyState handle replies
	void handleTcpReplies();

	//LobbyState handle replies
	void handleUdpReplies();

	//LobbyState map size
	sf::Vector2u mapSize;

	//LobbyState max clients;
	sf::Uint8 maxClients;

	//LobbyState client packet
	sf::Packet packet;

	//LobbyState lobby text
	sf::Text lobbyText;

	//LobbyState text box
	TextBox lobby;

	//LobbyState ready button
	Button readyButton;

	//LobbyState disconnect button
	Button disconnectButton;
};

#endif