#include "AStar.h"

int AStar::CheckClosedList(GridNode* node)
{
    for (int i = 0; i < closedList.size(); i++)
    {
        if (closedList[i] == node) return i;
    }
    return -1;
}

int AStar::CheckOpenList(GridNode* node)
{
    for (int i = 0; i < openList.size(); i++)
    {
        if (openList[i] == node) return i;
    }
    return -1;
}

void AStar::SetPath(GridNode* end)
{
    GridNode* node = end;
    while (node)
    {
        finalPath.insert(finalPath.begin(), node->position * 20);
        node->inPath = true;
        node = node->parent;
    }
}

GridNode* AStar::GetNeighbour(Grid& grid, GridNode* current, int dir)
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

float AStar::ManhattenHeuristic(GridNode* current, GridNode* goal)
{
    Vector2 vec = goal->position - current->position;     
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

AStar::AStar()
{
    finalPath.clear();
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

            if (!neigbour || !neigbour->walkable) continue;

            //stop cutting corners
            int j = i;
            i == 7 ? j = -1 : j = i;
            GridNode* right = GetNeighbour(grid, current, j + 1);
            GridNode * left = GetNeighbour(grid, current, i - 1);

            if (i % 2 != 0) if (left && left->walkable == false || right && right->walkable == false) continue;

            float dist = (neigbour->position - current->position).Magnitude();

            float tempH = ManhattenHeuristic(neigbour, goal);
            float tempG = current->gCost + dist;
            float tempF = tempG + tempH;

            int openCount = CheckOpenList(neigbour);
            int closedCount = CheckClosedList(neigbour);
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
            else if (closedCount != -1)
            {
                if (neigbour->fCost > tempF)
                {
                    closedList[closedCount]->gCost = tempG;
                    closedList[closedCount]->hCost = tempH;
                    closedList[closedCount]->fCost = tempF;
                    closedList[closedCount]->parent = current;
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
