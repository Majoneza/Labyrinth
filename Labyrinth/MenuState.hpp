#pragma once
#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.hpp"
#include "Button.hpp"

class MenuState : public GameState
{
public:
	//MenuState constructor
	MenuState(GameStateManager& gsm);

	//MenuState onEvent handler
	void onEvent(const sf::Event& evn) override;

	//MenuState onTick handler
	void onTick() override;

	//MenuState onRender handler
	void onRender(sf::RenderWindow& win) override;
private:
	//MenuState title
	sf::Text title;

	//MenuState "Singleplayer" Button
	Button singleplayerButton;

	//MenuState "Multiplayer" Button
	Button multiplayerButton;

	//MenuState "Quit" Button
	Button quitButton;
};

#endif