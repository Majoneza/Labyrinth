#pragma once
#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.hpp"
#include "Map.hpp"
#include "InputListener.hpp"
#include "Player.hpp"

class PlayState : public GameState
{
public:
	//PlayState constructor
	PlayState(GameStateManager& gsm);
	
	//PlayState onEvent handler
	void onEvent(const sf::Event& evn) override;

	//PlayState onTick handler
	void onTick() override;

	//PlayState onRender handler
	void onRender(sf::RenderWindow& win) override;
private:
	//PlayState create game
	void createGame();

	//PlayState map
	Map map;

	//PlayState input listener
	InputListener<Player, 4> listener;

	//PlayState player
	Player player;

	//PlayState timer
	sf::Clock timer;

	//PlayState time left
	float timeLeft;

	//PlayState level
	sf::Uint16 level;

	//PlayState time text
	sf::Text timeText;

	//PlayState level text
	sf::Text levelText;

	//PlayState start texture
	sf::Texture startTexture;

	//PlayState end texture
	sf::Texture endTexture;

	//PlayState wall texture
	sf::Texture wallTexture;
};

#endif