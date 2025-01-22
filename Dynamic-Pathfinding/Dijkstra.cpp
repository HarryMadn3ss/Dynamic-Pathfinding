#include "Dijkstra.h"

Dijkstra::Dijkstra()
{
	finalPath.clear();
}

bool Dijkstra::CreatePath(Grid& grid, Vector2 start, Vector2 goal)
{
	//reset vectors
	finalPath.clear();

	openList   = std::vector<GridNode*>();
	closedList = std::vector<GridNode*>();

	GridNode* current = grid.GetGridNode(start.x, start.y);

	openList.push_back(current);

	GridNode* goalNode = grid.GetGridNode(goal.x, goal.y);


	while (current != nullptr)
	{
		if (current != nullptr)
		{
			if (current == goalNode)
			{
				//construct path
				SetPath(current);
				return true;
				//return path
			}
		}

		//add all neighbours of current node to open list		
		for (int i = 0; i < 8; i++)
		{
			//if (GetNeighbour(grid, &current, i)) openList.push_back(GetNeighbour(grid, &current, i));
		
			GridNode* neighbour = GetNeighbour(grid, current, i);

			if (!neighbour) continue;
			if (!neighbour->walkable) continue;
			if (CheckClosedList(neighbour) > -1) continue;

			float dist = current->position.Magnitude();

			float cost = current->gCost + neighbour->gCost + dist;

			int openCount = CheckOpenList(neighbour);
			if (openCount != -1)
			{
				if (neighbour->fCost > cost)
				{
					openList[openCount]->parent = current;
					openList[openCount]->fCost = cost;
				}
			}
			else
			{
				GridNode* newNode = new GridNode(neighbour, current, cost);
				openList.push_back(newNode);
			}
		}

		closedList.push_back(current);
		openList.erase(openList.begin() + CheckOpenList(current));

		if (openList.size() > 0) current = openList[0];
		else current = nullptr;
	}

	return false;
}

GridNode* Dijkstra::GetNeighbour(Grid& grid, GridNode* current, int dir)
{
	GridNode* newNode = nullptr;
	switch (dir)
	{
	case 0://up
		newNode = grid.GetGridNode(current->position.x, current->position.y + 1);
		newNode->parent = current;
		break;
	case 1://upright
		newNode = grid.GetGridNode(current->position.x + 1, current->position.y + 1);
		newNode->parent = current;
		break;
	case 2: // right
		newNode = grid.GetGridNode(current->position.x + 1, current->position.y);
		newNode->parent = current;
		break;
	case 3: // down right
		newNode = grid.GetGridNode(current->position.x + 1, current->position.y - 1);
		newNode->parent = current;
		break;
	case 4://down
		newNode = grid.GetGridNode(current->position.x, current->position.y - 1);
		newNode->parent = current;
		break;
	case 5:// down left
		newNode = grid.GetGridNode(current->position.x - 1, current->position.y - 1);
		newNode->parent = current;
		break;
	case 6://left
		newNode = grid.GetGridNode(current->position.x - 1, current->position.y);
		newNode->parent = current;
		break;
	case 7 ://up left
		newNode = grid.GetGridNode(current->position.x - 1, current->position.y + 1);
		newNode->parent = current;
		break;
	default:
		break;
	}
	return newNode;
}

int Dijkstra::CheckClosedList(GridNode* node)
{
	for (int i = 0;	i < closedList.size(); i++)
	{
		if (closedList[i] == node) return i;
	}
	return -1;
}

int Dijkstra::CheckOpenList(GridNode* node)
{
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i] == node) return i;
	}
	return -1;
}

void Dijkstra::SetPath(GridNode* end)
{
	GridNode* node = end;

	while (node)
	{
		finalPath.insert(finalPath.begin(), node->position);
		node = node->parent;
	}
}


