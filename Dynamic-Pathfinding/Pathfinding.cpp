#include "Pathfinding.h"

int Pathfinding::CheckOpenList(GridNode* node)
{
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i] == node) return i;
	}
	return -1;
}

int Pathfinding::CheckClosedList(GridNode* node)
{
	for (int i = 0; i < closedList.size(); i++)
	{
		if (closedList[i] == node) return i;
	}
	return -1;
}

void Pathfinding::SetPath(GridNode* end)
{
	GridNode* node = end;
	while (node)
	{
		finalPath.insert(finalPath.begin(), (node->position * 20));		
		node->inPath = true;
		node = node->parent;
	}
}

GridNode* Pathfinding::GetNeighbour(Grid& grid, GridNode* current, int dir)
{
	GridNode* newNode = nullptr;
	switch (dir)
	{
	case 0://up
		newNode = grid.GetGridNode(current->position.x, current->position.y + 1);
		break;
	case 1://upright
		newNode = grid.GetGridNode(current->position.x + 1, current->position.y + 1);
		break;
	case 2: // right
		newNode = grid.GetGridNode(current->position.x + 1, current->position.y);
		break;
	case 3: // down right
		newNode = grid.GetGridNode(current->position.x + 1, current->position.y - 1);
		break;
	case 4://down
		newNode = grid.GetGridNode(current->position.x, current->position.y - 1);
		break;
	case 5:// down left
		newNode = grid.GetGridNode(current->position.x - 1, current->position.y - 1);
		break;
	case 6://left
		newNode = grid.GetGridNode(current->position.x - 1, current->position.y);
		break;
	case 7://up left
		newNode = grid.GetGridNode(current->position.x - 1, current->position.y + 1);
		break;
	default:
		break;
	}
	return newNode;
}

Pathfinding::Pathfinding()
{
	openList.clear();
	closedList.clear();
	finalPath.clear();
}

