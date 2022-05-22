#include "stdafx.h"
#include "GameState.hpp"
#include "Server.hpp"

GameState::GameState(GameStateManager& gsm) :
	gsm(gsm),
	server(gsm.server)
{
}