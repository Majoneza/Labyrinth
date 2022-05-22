#pragma once
#ifndef VERIFYSTATE_H
#define VERIFYSTATE_H

#include "GameState.hpp"
#include <SFML\Network.hpp>
#include "InputBox.hpp"
#include "Button.hpp"

class VerifyState : public GameState
{
public:
	//VerifyState constructor
	VerifyState(GameStateManager& gsm, const sf::String& username);
	
	//VerifyState destructor
	~VerifyState() = default;

	//VerifyState onEvent handler
	void onEvent(const sf::Event& evn) override;

	//VerifyState onTick handler
	void onTick() override;

	//VerifyState onRender handler
	void onRender(sf::RenderWindow& win) override;
private:
	//VerifyState handle replies
	void handleTcpReplies();

	//Verify confirm password
	void confirmPassword();

	//VerifyState text
	sf::Text text;

	//VerifyState title
	sf::Text title;

	//VerifyState password input box
	InputBox password;

	//VerifyState confirm button
	Button confirmButton;

	//VerifyState disconnect button
	Button disconnectButton;

	//VerifyState packet
	sf::Packet packet;

	//VerifyState hide password
	bool hidePassword;

	//VerifyState username
	const sf::String username;
};

#endif