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

void InputManager::HandleMouseClick(SDL_Event* e, Grid* grid, Agent& agent, Vector2 nextNode)
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
		if (node == grid->GetGridNode(nextNode.x, nextNode.y)) return;
		if (node && !node->curentGoal)
		{
			node->walkable = !node->walkable;
			if (!node->walkable) node->rhsCost = std::numeric_limits<float>::infinity();
			else node->rhsCost = 0.0f;
			node->curentGoal = false;
			node->inPath = false;
			node->searched = false;
		}
		break;
	case 2:
		agent.SetPos(Vector2(x, y));
		break;
	case 3: //right
		node = grid->GetGridNode(round(x) / 20, round(y) / 20);
		if (node && node != grid->goal)
		{
			node->curentGoal = true;
			node->walkable = true;
			grid->goal = node;
		}
		else if(node && node == grid->goal)
		{
			node->curentGoal = false;
			grid->goal = nullptr;
		}
		break;
	default:
		break;
	}
}
