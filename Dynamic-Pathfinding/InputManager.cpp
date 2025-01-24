#include "InputManager.h"

bool InputManager::HandleKeyInput(SDL_Event* e)
{
	switch (e->key.keysym.sym)
	{
	case SDLK_ESCAPE:
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

void InputManager::HandleMouseClick(SDL_Event* e, Grid* grid)
{
	//mouse pos
	int x, y;
	SDL_GetMouseState(&x, &y);
	GridNode* node = NULL;
	switch (e->button.button)
	{
	case 1: // left
		//get the grid node its closest to
		node =  grid->GetGridNode(round(x) / 20, round(y) / 20);
		if (node)
		{
			node->walkable = !node->walkable;
			node->curentGoal = false;
			node->inPath = false;
			node->searched = false;
		}
		break;
	case 3: //right
		node = grid->GetGridNode(round(x) / 20, round(y) / 20);
		if (node != grid->goal)
		{
			node->curentGoal = true;
			node->walkable = true;
			grid->goal = node;
		}
		else if(node == grid->goal)
		{
			node->curentGoal = false;
			grid->goal = nullptr;
		}
		break;
	default:
		break;
	}
}
