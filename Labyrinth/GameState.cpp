#include "stdafx.h"
#include "GameState.hpp"
#include "GameStateManager.hpp"
#include "Game.hpp"

GameState::GameState(GameStateManager& gsm) :
	gsm(gsm),
	game(gsm.game),
	client(gsm.game.client),
	config(gsm.game.config)
{
}