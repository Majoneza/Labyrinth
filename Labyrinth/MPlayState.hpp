#pragma once
#ifndef MPLAYSTATE_H
#define MPLAYSTATE_H

#include "GameState.hpp"
#include <SFML\Network.hpp>
#include "Map.hpp"
#include "Player.hpp"
#include "InputListener.hpp"
#include "dynarray.hpp"
#include "TextBox.hpp"

class MPlayState : public GameState
{
public:
	//MPlayState constructor
	MPlayState(GameStateManager& gsm, const sf::Vector2u& map_size, const sf::Uint8& client_number);

	//MPlayState onEvent handler
	void onEvent(const sf::Event& evn) override;

	//MPlayState onTick handler
	void onTick() override;

	//MPlayState onRender handler
	void onRender(sf::RenderWindow& win) override;
private:
	//MPlayState handle udp replies
	void handleUdpReplies();

	//MPlayState handle tcp replies
	void handleTcpReplies();

	//MPlayState move up
	void moveUp();

	//MPlayState move down
	void moveDown();

	//MPlayState move left
	void moveLeft();

	//MPlayState move right
	void moveRight();

	//MPlayState max clients
	sf::Uint8 maxClients;

	//MPlayState time text
	sf::Text timeText;

	//MPlayState text box
	TextBox scores;

	//MPlayState map
	Map map;

	//MPlayState player
	Player player;

	//MPlayState input listener
	InputListener<MPlayState, 4> listener;

	//MPlayState opponents
	dynarray<Player> opponents;

	//MPlayState start texture
	sf::Texture startTexture;

	//MPlayState end texture
	sf::Texture endTexture;

	//MPlayState wall texture
	sf::Texture wallTexture;

	//MPlayState packet
	sf::Packet packet;
};

#endif