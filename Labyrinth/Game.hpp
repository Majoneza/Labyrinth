#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML\Graphics.hpp>
#include <nlohmann\json.hpp>
#include "GameStateManager.hpp"
#include "Client.hpp"

class Game
{
public:
	//Game default constructor
	Game(const sf::String& title, const sf::Uint32& style, const sf::ContextSettings& settings, const sf::String& configDir);

	//Game config
	nlohmann::json config;

	//Game fonts
	struct {
		//Game digital font
		sf::Font digital;

		//Game ailerons font
		sf::Font ailerons;

		//Game jura font
		sf::Font jura;

		//Game openSans font
		sf::Font openSans;
	} fonts;

	//Game client socket
	Client client;

	//Game quit
	void quit();
private:
	//GameStateManager 
	GameStateManager gsm;

	//Game window
	sf::RenderWindow window;

	//Game load config
	bool loadConfig(const sf::String& file);

	//Game onEvent handler
	void onEvent(const sf::Event& evn) const;

	//Game onTick handler
	void onTick() const;

	//Game onRender handler
	void onRender(sf::RenderWindow& win) const;

	//Game run function
	void run();
};

#endif