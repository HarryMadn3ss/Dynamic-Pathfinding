#include "InputManager.h"

bool InputManager::HandleKeyInput(SDL_Event& e)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		return true;
		break;
	default:
		break;
	}
}
