#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML\Graphics.hpp>
#include <nlohmann\json.hpp>

class Client;
class Game;
class GameStateManager;
class GameState
{
public:
	//GameState constructor
	GameState(GameStateManager& gsm);

	//GameState destructor
	virtual ~GameState() = default;

	//GameState onEvent handler
	virtual void onEvent(const sf::Event& evn) = 0;

	//GameState onTick handler
	virtual void onTick() = 0;

	//GameState onRender handler
	virtual void onRender(sf::RenderWindow& win) = 0;
protected:
	//GameState GameStateManager
	GameStateManager & gsm;

	//GameState game
	Game& game;

	//GameState client
	Client& client;

	//GameState config
	nlohmann::json& config;
};

#endif