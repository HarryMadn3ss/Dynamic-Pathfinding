#include "DStar.h"

float DStar::ManhattenHueristic(GridNode current, GridNode goal)
{
	Vector2 vec = goal.position - current.position;
	return abs(vec.x) + abs(vec.y);
}

GridNode* DStar::GetCheapestNode()
{
    if (openList.size() == 0) return nullptr;
    GridNode* cheapest = openList[0];
    for (int i = 0; i < openList.size(); i++)
    {
        if (openList[i]->fCost < cheapest->fCost) cheapest = openList[i];
    }
    return cheapest;
}

bool DStar::CheckConsistency(GridNode& node)
{    
    return node.gCost == node.rhsCost;
}

float DStar::GetTravelCost(int dir)
{
    if (dir % 2 == 0) return 1.0f;
    return 1.4f;
}

bool DStar::CreatePath(Grid& grid, Vector2 start)
{
    //clear lists
    finalPath.clear();
    openList.clear();
    closedList.clear();

    //D*Lite works backwards
    GridNode* current = grid.goal;  

    current->gCost = 0.0f;
    current->rhsCost = 0.0f;
    current->parent = nullptr;

    openList.push_back(current);

    GridNode* goal = grid.GetGridNode(start.x, start.y);
    goal->parent = nullptr;

    while (current)
    {
        if (current == goal)
        {
            SetPath(current);
            return true;
        }

        for (int i = 0; i < 8; i++)
        {
            GridNode* neighbour = GetNeighbour(grid, current, i);

            int closedCount = CheckClosedList(neighbour);
            if (!neighbour || !neighbour->walkable || closedCount != -1) continue;

            //stop cutting corners
            int j = i;
            i == 7 ? j = -1 : j = i;
            GridNode* right = GetNeighbour(grid, current, j + 1);
            GridNode* left = GetNeighbour(grid, current, i - 1);

            if (i % 2 != 0) if (left && left->walkable == false || right && right->walkable == false) continue;

            float dist = (neighbour->position - current->position).Magnitude();

            float tempH = ManhattenHueristic(*neighbour, *goal);
            float tempG = current->gCost + GetTravelCost(i) + dist;
            float tempF = tempG + tempH;

            int openCount = CheckOpenList(neighbour);
            if (openCount != -1)
            {
                if (neighbour->fCost > tempF)
                {
                    openList[openCount]->gCost = tempG;
                    openList[openCount]->hCost = tempH;
                    openList[openCount]->fCost = tempF;
                    openList[openCount]->rhsCost = tempG;
                    openList[openCount]->parent = current;
                }
            }
            else
            {
                neighbour->gCost = tempG;
                neighbour->hCost = tempH;
                neighbour->fCost = tempF;
                neighbour->rhsCost = tempG;
                neighbour->parent = current;
                openList.push_back(neighbour);
            }

        }

        current->searched = true;
        closedList.push_back(current);
        openList.erase(openList.begin() + CheckOpenList(current));

        if (openList.size() > 0) current = GetCheapestNode();
        else current = nullptr;
    }
    return false;

    return false;
}

bool DStar::CreatePath(Grid& grid, Vector2 start, Vector2 _goal)
{
	//D*Lite works backwards
    GridNode* current = nullptr;

    if (_goal == Vector2(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()))
    {
        current = grid.goal;
    }
    else current = grid.GetGridNode(_goal.x, _goal.y);

    current->gCost = 0.0f;
    current->rhsCost = 0.0f;
	current->parent = nullptr;

	openList.push_back(current);

	GridNode* goal = grid.GetGridNode(start.x, start.y);
	goal->parent = nullptr;

	while (current)
	{
		if (current == goal) 
		{
			SetPath(current);
			return true;
		}

		for (int i = 0; i < 8; i++)
		{
			GridNode* neighbour = GetNeighbour(grid, current, i);

            int closedCount = CheckClosedList(neighbour);
            if (!neighbour || !neighbour->walkable || closedCount != -1) continue;

            //stop cutting corners
            int j = i;
            i == 7 ? j = -1 : j = i;
            GridNode* right = GetNeighbour(grid, current, j + 1);
            GridNode* left = GetNeighbour(grid, current, i - 1);

            if (i % 2 != 0) if (left && left->walkable == false || right && right->walkable == false) continue;

            float dist = (neighbour->position - current->position).Magnitude();

            float tempH = ManhattenHueristic(*neighbour, *goal);
            float tempG = current->gCost + GetTravelCost(i) + dist;
            float tempF = tempG + tempH;

            int openCount = CheckOpenList(neighbour);
            if (openCount != -1)
            {
                if (neighbour->fCost > tempF)
                {
                    openList[openCount]->gCost = tempG;
                    openList[openCount]->hCost = tempH;
                    openList[openCount]->fCost = tempF;
                    openList[openCount]->rhsCost = tempG;
                    openList[openCount]->parent = current;
                }
            }
            else
            {
                neighbour->gCost = tempG;
                neighbour->hCost = tempH;
                neighbour->fCost = tempF;
                neighbour->rhsCost = tempG;
                neighbour->parent = current;
                openList.push_back(neighbour);
            }

        }

        current->searched = true;
        closedList.push_back(current);
        openList.erase(openList.begin() + CheckOpenList(current));

        if (openList.size() > 0) current = GetCheapestNode();
        else current = nullptr;
    }
	return false;
}

bool DStar::MoveForward(Grid& grid, Agent& agent)
{
    //if node infront is locally incosistant the goal is now set to inconsistant's parent and the start is the current 
    if (!CheckConsistency(*grid.GetGridNode(finalPath[finalPath.size() - 1].x, finalPath[finalPath.size() - 1].y)))
    {
        CreatePath(grid, agent.GetPos(), Vector2(finalPath[finalPath.size() - 2].x, finalPath[finalPath.size() - 2].y));
    }
    
    return true;
}
