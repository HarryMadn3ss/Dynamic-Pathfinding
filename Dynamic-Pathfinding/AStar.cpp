#include "AStar.h"

float AStar::ManhattenHeuristic(GridNode current, GridNode goal)
{
    Vector2 vec = goal.position - current.position;     
    return abs(vec.x) + abs(vec.y);
}

GridNode* AStar::GetCheapestNode()
{
    if (openList.size() == 0) return nullptr;
    GridNode* cheapest = openList[0];
    for (int i = 0; i < openList.size(); i++)
    {
        if (openList[i]->fCost < cheapest->fCost) cheapest = openList[i];
    }
    return cheapest;
}

bool AStar::CreatePath(Grid& grid, Vector2 start)
{
    //clear lists
    finalPath.clear();
    openList.clear();
    closedList.clear();

    GridNode* current = grid.GetGridNode(start.x, start.y);
    current->parent = nullptr;

    openList.push_back(current);
    
    GridNode* goal = grid.goal;
    goal->parent = nullptr;

    while (current != nullptr)
    {
        if (current == goal)
        {
            SetPath(current);
            return true;
        }

        for (int i = 0; i < 8; i++)
        {
            //stop diag
            //if (i % 2 != 0) continue;

            GridNode* neigbour = GetNeighbour(grid, current, i);

            int closedCount = CheckClosedList(neigbour);
            if (!neigbour || !neigbour->walkable || closedCount != -1) continue;

            //stop cutting corners
            int j = i;
            i == 7 ? j = -1 : j = i;
            GridNode* right = GetNeighbour(grid, current, j + 1);
            GridNode * left = GetNeighbour(grid, current, i - 1);

            if (i % 2 != 0) if (left && left->walkable == false || right && right->walkable == false) continue;

            float dist = (neigbour->position - current->position).Magnitude();

            float tempH = ManhattenHeuristic(*neigbour, *goal);
            float tempG = current->gCost + neigbour->gCost + dist;
            float tempF = tempG + tempH;

            int openCount = CheckOpenList(neigbour);
            if (openCount != -1)
            {
                if (neigbour->fCost > tempF)
                {
                    openList[openCount]->gCost = tempG;
                    openList[openCount]->hCost = tempH;
                    openList[openCount]->fCost = tempF;
                    openList[openCount]->parent = current;
                }
            }
            else
            {
                neigbour->gCost = tempG;
                neigbour->hCost = tempH;
                neigbour->fCost = tempF;
                neigbour->parent = current;
                openList.push_back(neigbour);
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
