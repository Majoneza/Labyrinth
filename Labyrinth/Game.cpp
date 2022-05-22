#include "stdafx.h"
#include "Game.hpp"
#include "MenuState.hpp"
#include <fstream>
#include <Windows.h>

Game::Game(const sf::String& title, const sf::Uint32& style, const sf::ContextSettings& settings, const sf::String& configDir) :
	gsm(*this)
{
	if (!loadConfig(configDir)) { printf("Cannot load config!\n"); return; }
	if (!config["Window"]["show_console"]) ShowWindow(GetConsoleWindow(), SW_HIDE);
	fonts.digital.loadFromFile(config["Fonts"]["digital"]);
	fonts.ailerons.loadFromFile(config["Fonts"]["ailerons"]);
	fonts.jura.loadFromFile(config["Fonts"]["jura"]);
	fonts.openSans.loadFromFile(config["Fonts"]["open_sans"]);
	window.create(sf::VideoMode(config["Window"]["width"], config["Window"]["height"]), title, style, settings);
	gsm.setState(new MenuState(gsm));
	run();
}

void Game::quit()
{
	window.close();
}

bool Game::loadConfig(const sf::String& file)
{
	try {
		config = nlohmann::json::parse(std::ifstream(std::string(file)));
		return true;
	}
	catch (std::exception&)
	{
		return false;
	}
}

void Game::onEvent(const sf::Event& evn) const
{
	gsm.onEvent(evn);
}

void Game::onTick() const
{
	gsm.onTick();
}

void Game::onRender(sf::RenderWindow& win) const
{
	gsm.onRender(win);
}

void Game::run()
{
	window.setFramerateLimit(120);
	while (window.isOpen())
	{
		/*
		EVENT HANDLER
		*/
		sf::Event evn;
		while (window.pollEvent(evn))
		{
			//If window closed
			if (evn.type == sf::Event::Closed) window.close();
			//onEvent
			onEvent(evn);
		}

		/*
		TICK AND RENDER
		*/

		onTick();

		window.clear(sf::Color::White);
		onRender(window);
		window.display();
	}
}