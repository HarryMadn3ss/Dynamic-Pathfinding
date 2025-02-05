#pragma once
#include <SDL.h>
#include "Grid.h"
#include "Agent.h"

namespace InputManager
{
	//handle key input
	bool HandleKeyInput(SDL_Event* e);
	void HandleMouseClick(SDL_Event* e, Grid* grid, Agent& agent);
};

