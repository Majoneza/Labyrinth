#include "stdafx.h"
#include "JoinState.hpp"
#include "MenuState.hpp"
#include "VerifyState.hpp"
#include "Game.hpp"

JoinState::JoinState(GameStateManager& gsm) :
	GameState(gsm),
	title(
		"Join Game", 
		game.fonts.digital, 
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) * 0.5f) * 0.08f)
	),
	status(
		"",
		game.fonts.digital,
		static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) * 0.5f) * 0.04f)
	),
	username(
		float(config["Window"]["width"]) * 0.5f, 
		float(config["Window"]["height"]) * 0.38f, 
		float(config["Window"]["width"]) * 0.3f, 
		float(config["Window"]["height"]) * 0.065f,
		sf::Color::Black, 
		sf::Color::Transparent, 
		sf::Color::Black, 
		1.f, 
		game.fonts.jura, 
		16U, 
		InputBox::LETTERS_NUMBERS, 
		"Username"
	),
	ip(
		float(config["Window"]["width"]) * 0.5f, 
		float(config["Window"]["height"]) * 0.48f, 
		float(config["Window"]["width"]) * 0.3f, 
		float(config["Window"]["height"]) * 0.065f,
		sf::Color::Black, 
		sf::Color::Transparent, 
		sf::Color::Black, 
		1.f, 
		game.fonts.jura, 
		15U, 
		InputBox::IP, 
		"IP"
	),
	port(
		float(config["Window"]["width"]) * 0.5f, 
		float(config["Window"]["height"]) * 0.58f, 
		float(config["Window"]["width"]) * 0.3f, 
		float(config["Window"]["height"]) * 0.065f,
		sf::Color::Black, 
		sf::Color::Transparent, 
		sf::Color::Black, 
		1.f, 
		game.fonts.jura, 
		15U, 
		InputBox::NUMBERS, 
		"Port"
	),
	joinButton(
		float(config["Window"]["width"]) / 2.f, 
		float(config["Window"]["height"]) * 0.8f, 
		float(config["Window"]["width"]) * 0.25f, 
		float(config["Window"]["height"]) * 0.065f, 
		sf::Color::Color(96, 96, 96), 
		sf::Color::Color(255, 229, 204), 
		"Join", 
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
	title.setFillColor(sf::Color::Black);
	title.setOrigin(title.getGlobalBounds().width * 0.5f, title.getGlobalBounds().height * 0.5f);
	title.setPosition(float(config["Window"]["width"]) * 0.5f, float(config["Window"]["height"]) * 0.1f);
	status.setFillColor(sf::Color::Black);
	status.setOrigin(status.getGlobalBounds().width * 0.5f, status.getGlobalBounds().height * 0.5f);
	status.setPosition(float(config["Window"]["width"]) * 0.5f, float(config["Window"]["height"]) * 0.67f);
	joinButton.setTextSize(static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.035f));
	menuButton.setTextSize(static_cast<sf::Uint16>(((float(config["Window"]["width"]) + float(config["Window"]["height"])) / 2.f) * 0.035f));
}

void JoinState::onEvent(const sf::Event& evn)
{
	username.onEvent(evn);
	ip.onEvent(evn);
	port.onEvent(evn);
	joinButton.onEvent(evn);
	menuButton.onEvent(evn);

	if (evn.type == sf::Event::KeyPressed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
		{
			if (username.isFocused())
			{
				username.unfocus();
				ip.focus();
			}
			else if (ip.isFocused())
			{
				ip.unfocus();
				port.focus();
			}
			else if (port.isFocused())
			{
				port.unfocus();
				username.focus();
			}
			else username.focus();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) connect();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) gsm.setState(new MenuState(gsm));
}

void JoinState::onTick()
{
	if (joinButton.isPressed()) connect();
	else if (menuButton.isPressed()) gsm.setState(new MenuState(gsm));
}

void JoinState::onRender(sf::RenderWindow& win)
{
	username.onRender(win);
	ip.onRender(win);
	port.onRender(win);
	joinButton.onRender(win);
	menuButton.onRender(win);
	win.draw(title);
	win.draw(status);
}

void JoinState::connect()
{
	if (username.getInputLength() <= 0U) username.setTemporaryOutlineColor(sf::Color::Red);
	else if (ip.getInputLength() <= 0U) ip.setTemporaryOutlineColor(sf::Color::Red);
	else if (port.getInputLength() <= 0U || port.getInput<sf::Int32>() == 0) port.setTemporaryOutlineColor(sf::Color::Red);
	else
	{
		client.tcp.setBlocking(true);

		status.setString("");

		switch (client.tcp.connect(ip.getInput<std::string>(), port.getInput<sf::Int32>(), sf::milliseconds(config["Network"]["connect_timeout"])))
		{
		case sf::Socket::Status::Done:
			client.serverAddress = ip.getInput<std::string>();
			client.tcp.setBlocking(false);
			printf("Connected\n");
			status.setString("Connected\n");
			gsm.setState(new VerifyState(gsm, username.getInput<sf::String>()));
			break;
		case sf::Socket::Status::Error:
			printf("Connect socket error\n");
			status.setString("Failed to connect\n");
			break;
		case sf::Socket::Status::NotReady:
			printf("Connect socket not ready\n");
			status.setString("Failed to connect\n");
			break;
		case sf::Socket::Status::Partial:
			printf("Connect socket partial\n");
			status.setString("Failed to connect\n");
			break;
		case sf::Socket::Status::Disconnected:
			status.setString("Failed to connect\n");
			break;
		}
		status.setOrigin(status.getGlobalBounds().width * 0.5f, status.getGlobalBounds().height * 0.5f);
		status.setPosition(float(config["Window"]["width"]) * 0.5f, float(config["Window"]["height"]) * 0.67f);
	}
}