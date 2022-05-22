#include "stdafx.h"
#include "PlayState.hpp"
#include "GameStateManager.hpp"
#include "GameOverState.hpp"
#include "Game.hpp"
#include <SFML\System\Time.hpp>

PlayState::PlayState(GameStateManager& gsm) :
	GameState(gsm),
	map(
		config["Map"]["size"],
		config["Map"]["size"],
		config["Window"]["width"],
		config["Window"]["height"]
	),
	listener(
		player,
		true,
		{
		{ config["Player"]["up_input"], { &Player::moveUp } },
		{ config["Player"]["down_input"], { &Player::moveDown } },
		{ config["Player"]["left_input"], { &Player::moveLeft } },
		{ config["Player"]["right_input"], { &Player::moveRight } }
	}),
	player(
		config["Player"]["sprite"].get<std::string>(),
		map.startPos,
		map.getTileSize(), 
		&map
	),
	timeLeft(
		config["Map"]["size"].get<float>() / config["Mode"]["difficulty"].get<float>()
	),
	level(0U),
	timeText(
		"Time: ?", 
		game.fonts.digital, 
		static_cast<sf::Uint16>(((config["Window"]["width"].get<float>() + config["Window"]["height"].get<float>()) / 2.f) * 0.04f)
	),
	levelText(
		"Level: ?", 
		game.fonts.digital, 
		static_cast<sf::Uint16>(((config["Window"]["width"].get<float>() + config["Window"]["height"].get<float>()) / 2.f) * 0.04f)
	)
{
	startTexture.loadFromFile(config["Tile"]["start_sprite"].get<std::string>());
	endTexture.loadFromFile(config["Tile"]["end_sprite"].get<std::string>());
	wallTexture.loadFromFile(config["Tile"]["wall_sprite"].get<std::string>());
	levelText.setFillColor(sf::Color::Black);
	levelText.setOrigin(levelText.getGlobalBounds().width / 2.f, levelText.getGlobalBounds().height / 2.f);
	levelText.setPosition(config["Window"]["width"].get<float>() / 2.f, levelText.getGlobalBounds().height / 2.f);
	timeText.setFillColor(sf::Color::Red);
	timeText.setOrigin(timeText.getGlobalBounds().width / 2.f, timeText.getGlobalBounds().height / 2.f);
	timeText.setPosition(config["Window"]["width"].get<float>() / 2.f, config["Window"]["height"].get<float>() - timeText.getGlobalBounds().height);
	createGame();
}

void PlayState::onEvent(const sf::Event& evn)
{
	listener.onEvent(evn);
	map.onEvent(evn);
	player.onEvent(evn);
}

void PlayState::onTick()
{
	map.onTick();
	player.onTick();
	if (player.getPosition() == map.endPos) createGame();
	if (timeLeft - timer.getElapsedTime().asSeconds() <= 0) gsm.setState(new GameOverState(gsm, level));
	else timeText.setString(std::string("Time: ") + std::to_string(int(timeLeft - timer.getElapsedTime().asSeconds())));
}

void PlayState::onRender(sf::RenderWindow& win)
{
	map.onRender(win);
	player.onRender(win);
	win.draw(levelText);
	win.draw(timeText);
}

void PlayState::createGame()
{
	timeLeft -= timer.getElapsedTime().asSeconds();
	map.createMap(startTexture, endTexture, wallTexture);
	player.setPosition(map.startPos);
	timeLeft += (map.getSize().x + map.getSize().y) / config["Mode"]["difficulty"].get<float>();
	levelText.setString("Level: " + std::to_string(++level));
	timer.restart();
}