#include "Dijkstra.h"

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
		if (current == goalNode)
		{			
			SetPath(current);
			return true;				
		}

		//add all neighbours of current node to open list		
		for (int i = 0; i < 8; i++)
		{			
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