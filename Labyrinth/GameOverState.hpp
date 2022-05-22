#pragma once
#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.hpp"
#include "Button.hpp"

class GameOverState : public GameState
{
public:
	//GameOverState constructor
	GameOverState(GameStateManager& gsm, const sf::Uint32& score);

	//GameOverState onEvent handler
	void onEvent(const sf::Event& evn) override;

	//GameOverState onTick handler
	void onTick() override;

	//GameOverState onRender handler
	void onRender(sf::RenderWindow& win) override;
private:
	//GameOverState gameover text
	sf::Text gameOverText;

	//GameOverState level text
	sf::Text level;

	//GameOverState back to menu button
	Button menuButton;

	//GameOverState retry button
	Button retryButton;
};

#endif