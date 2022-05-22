#pragma once
#ifndef MGAMEOVERSTATE_H
#define MGAMEOVERSTATE_H

#include "GameState.hpp"
#include <SFML/Network/Packet.hpp>
#include "Button.hpp"
#include "TextBox.hpp"

class MGameOverState : public GameState
{
public:
	//MGameOverState constructor
	MGameOverState(GameStateManager& gsm, const sf::Uint8& client_number);

	//MGameOverState onEvent handler
	void onEvent(const sf::Event& evn) override;

	//MGameOverState onTick handler
	void onTick() override;

	//MGameOverState onRender handler
	void onRender(sf::RenderWindow& win) override;
private:
	//MGameOverState handle tcp replies
	void handleTcpReplies();

	//MGameOverState max clients;
	sf::Uint8 maxClients;

	//MGameOverState lobby text
	sf::Text gameOverText;

	//MGameOverState text box
	TextBox results;

	//MGameOverState menu button
	Button menuButton;

	sf::Packet packet;
};

#endif