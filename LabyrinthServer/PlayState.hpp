#pragma once
#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.hpp"

class PlayState : public GameState
{
public:
	PlayState(GameStateManager& gsm);

	void onTick() override;
private:
	bool isGameOver() const;
};

#endif