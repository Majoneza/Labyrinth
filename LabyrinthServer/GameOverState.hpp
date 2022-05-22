#pragma once
#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.hpp"

class GameOverState : public GameState
{
public:
	GameOverState(GameStateManager& gsm);

	void onTick() override;
private:
	void reset() const;
};

#endif