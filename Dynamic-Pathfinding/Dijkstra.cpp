#include "Dijkstra.h"

Dijkstra::Dijkstra()
{
	finalPath.clear();
}

bool Dijkstra::CreatePath(Grid& grid, Vector2 start)
{
	//reset vectors
	finalPath.clear();

	openList.clear();
	closedList.clear();

	GridNode* current = grid.GetGridNode(start.x, start.y);
	current->parent = nullptr;

	openList.push_back(current);

	GridNode* goalNode = grid.goal;
	goalNode->parent = nullptr;


	while (current != nullptr)
	{
		//if (current != nullptr)
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
			//stop diag
			//if (i % 2 != 0) continue;
		
			GridNode* neighbour = GetNeighbour(grid, current, i);		

			if (!neighbour || !neighbour->walkable || CheckClosedList(neighbour) > -1) continue;
			//stop cutting corners
			int j = i;
			i == 7 ? j = -1 : j = i;
			GridNode* right = GetNeighbour(grid, current, j + 1);
			GridNode* left = GetNeighbour(grid, current, i - 1);
			if (i % 2 != 0) if (left && left->walkable == false || right && right->walkable == false) continue;

			float dist = (neighbour->position - current->position).Magnitude();

			float cost = current->fCost + neighbour->gCost + dist;

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
				neighbour->fCost = cost;
				neighbour->parent = current;			
				openList.push_back(neighbour);
			}
		}

		current->searched = true;
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
	case 7 ://up left
		newNode = grid.GetGridNode(current->position.x - 1, current->position.y + 1);
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
		finalPath.insert(finalPath.begin(), node->position * 20);
		node->inPath = true;
		node = node->parent;
	}
}


