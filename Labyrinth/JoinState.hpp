#pragma once
#ifndef JOINSTATE_H
#define JOINSTATE_H

#include "GameState.hpp"
#include "InputBox.hpp"
#include "Button.hpp"

class JoinState : public GameState
{
public:
	//JoinState constructor
	JoinState(GameStateManager& gsm);

	//JoinState destructor
	~JoinState() = default;

	//JoinState onEvent handler
	void onEvent(const sf::Event& evn) override;

	//JoinState onTick handler
	void onTick() override;

	//JoinState onRender handler
	void onRender(sf::RenderWindow& win) override;
private:
	//JoinState connect
	void connect();

	//JoinState title
	sf::Text title;

	//JoinState connection status
	sf::Text status;

	//JoinState username input box
	InputBox username;

	//JoinState ip input box
	InputBox ip;

	//JoinState port input box
	InputBox port;

	//JoinState join button
	Button joinButton;

	//JoinState menu button
	Button menuButton;
};
#endif