#include "stdafx.h"
#include "GameOverState.hpp"
#include "GameStateManager.hpp"
#include "Game.hpp"
#include "PlayState.hpp"
#include "MenuState.hpp"

GameOverState::GameOverState(GameStateManager& gsm, const sf::Uint32& score) :
	GameState(gsm),
	gameOverText(
		"Game Over!", 
		game.fonts.ailerons, 
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.07f)
	),
	level(
		"You reached Level: " + std::to_string(score) + "!", 
		game.fonts.ailerons, 
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.07f)
	),
	retryButton(
		float(config["Window"]["width"]) / 2.f, 
		float(config["Window"]["height"]) * 0.8f, 
		float(config["Window"]["width"]) * 0.25f, 
		float(config["Window"]["height"]) * 0.065f, 
		sf::Color::Color(96, 96, 96), 
		sf::Color::Color(255, 229, 204), 
		"Retry", 
		sf::Color::Color(255, 253, 209), 
		game.fonts.ailerons
	),
	menuButton(
		float(config["Window"]["width"]) / 2.f, 
		float(config["Window"]["height"]) * 0.9f, 
		float(config["Window"]["width"]) * 0.25f, 
		float(config["Window"]["height"]) * 0.065f, 
		sf::Color::Color(96, 96, 96), 
		sf::Color::Color(255, 229, 204), 
		"Menu", 
		sf::Color::Color(255, 253, 209), 
		game.fonts.ailerons
	)
{
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setOrigin(gameOverText.getGlobalBounds().width / 2.f, gameOverText.getGlobalBounds().height / 2.f);
	gameOverText.setPosition(float(config["Window"]["width"]) / 2.f, float(config["Window"]["height"]) * 0.3f);
	level.setFillColor(sf::Color::Black);
	level.setOrigin(level.getGlobalBounds().width / 2.f, level.getGlobalBounds().height / 2.f);
	level.setPosition(float(config["Window"]["width"]) / 2.f, float(config["Window"]["height"]) * 0.4f);
	retryButton.setTextSize(static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.035f));
	menuButton.setTextSize(static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.035f));
}

void GameOverState::onEvent(const sf::Event& evn)
{
	menuButton.onEvent(evn);
	retryButton.onEvent(evn);
}

void GameOverState::onTick()
{
	if (menuButton.isPressed()) gsm.setState(new MenuState(gsm));
	if (retryButton.isPressed()) gsm.setState(new PlayState(gsm));
}

void GameOverState::onRender(sf::RenderWindow& win)
{
	menuButton.onRender(win);
	retryButton.onRender(win);
	win.draw(gameOverText);
	win.draw(level);
}