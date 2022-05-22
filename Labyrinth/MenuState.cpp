#include "stdafx.h"
#include "MenuState.hpp"
#include "GameStateManager.hpp"
#include "Game.hpp"
#include "PlayState.hpp"
#include "JoinState.hpp"

MenuState::MenuState(GameStateManager& gsm) :
	GameState(gsm),
	singleplayerButton(
		float(config["Window"]["width"]) * 0.5f, 
		float(config["Window"]["height"]) * 0.38f, 
		float(config["Window"]["width"]) * 0.25f, 
		float(config["Window"]["height"]) * 0.065f, 
		sf::Color::Color(96, 96, 96), 
		sf::Color::Color(255, 229, 204), 
		"Singleplayer", 
		sf::Color::Color(255, 253, 209), 
		game.fonts.ailerons
	),
	multiplayerButton(
		float(config["Window"]["width"]) * 0.5f, 
		float(config["Window"]["height"]) * 0.5f, 
		float(config["Window"]["width"]) * 0.25f, 
		float(config["Window"]["height"]) * 0.065f, 
		sf::Color::Color(96, 96, 96),
		sf::Color::Color(255, 229, 204), 
		"Multiplayer", 
		sf::Color::Color(255, 253, 209), 
		game.fonts.ailerons
	),
	quitButton(
		float(config["Window"]["width"]) * 0.5f, 
		float(config["Window"]["height"]) * 0.62f, 
		float(config["Window"]["width"]) * 0.25f, 
		float(config["Window"]["height"]) * 0.065f, 
		sf::Color::Color(96, 96, 96), 
		sf::Color::Red, 
		"Quit", 
		sf::Color::Color(255, 253, 209), 
		game.fonts.ailerons
	),
	title(
		"Labyrinth", 
		game.fonts.digital, 
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) * 0.5f) * 0.08f)
	)
{
	title.setFillColor(sf::Color::Black);
	title.setOrigin(title.getGlobalBounds().width * 0.5f, title.getGlobalBounds().height * 0.5f);
	title.setPosition(float(config["Window"]["width"]) * 0.5f, float(config["Window"]["height"]) * 0.1f);
	singleplayerButton.setTextSize(static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.035f));
	multiplayerButton.setTextSize(static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.035f));
	quitButton.setTextSize(static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.035f));
}

void MenuState::onEvent(const sf::Event& evn)
{
	singleplayerButton.onEvent(evn);
	multiplayerButton.onEvent(evn);
	quitButton.onEvent(evn);
}

void MenuState::onTick()
{
	if (singleplayerButton.isPressed()) gsm.setState(new PlayState(gsm));
	if (multiplayerButton.isPressed()) gsm.setState(new JoinState(gsm));
	if (quitButton.isPressed()) gsm.game.quit();
}

void MenuState::onRender(sf::RenderWindow& win)
{
	singleplayerButton.onRender(win);
	multiplayerButton.onRender(win);
	quitButton.onRender(win);
	win.draw(title);
}