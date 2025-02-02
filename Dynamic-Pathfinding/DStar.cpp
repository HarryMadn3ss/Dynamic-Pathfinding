#include "DStar.h"

int DStar::CheckChangedNeighbours(GridNode* node)
{
    for (int i = 0; i < neighboursVec.size(); i++)
    {
        if (neighboursVec[i] == node) return i;
    }
    return -1;
}

void DStar::AmmendPath(GridNode* node)
{
    int count = 0;
    finalPath.pop_back();
    GridNode* cur = node;
    while (node)
    {
        finalPath.insert(finalPath.end() - count, (node->position * 20));
        node->inPath = true;
        node = node->parent;
        count++;
    }
}

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
    //if (node == nullptr)
    //{
    //    printf("node was null");
    //    return true;
    //}
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

    GridNode* goal = grid.GetGridNode(start.x / 20, start.y / 20);
    if (!goal)
    {
        printf("Goal is missing");
        return false;
    }
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

bool DStar::RecalculatePath(Grid& grid, Vector2 start, Vector2 _goal)
{
    //dont clear finalPath just need to update it
    openList.clear();
    closedList.clear();
    //new vector for the neighbours to check againest // could be array but not garunteered to be 8 viable nodes
    neighboursVec.clear();

    //new goal is node after the changed node
    GridNode* current = grid.GetGridNode(_goal.x / 20, _goal.y / 20);
    current->gCost = 0.0f;
    current->rhsCost = 0.0f;
	current->parent = nullptr;

	openList.push_back(current);

    GridNode* goal = grid.GetGridNode(start.x / 20, start.y / 20);
    goal->gCost = 0.0f;
    goal->rhsCost = 0.0f;
    goal->parent = nullptr;


    //need to add changed nodes to the open list and construct a new path
    GridNode* changedNode = grid.GetGridNode(finalPath[finalPath.size() - 1].x / 20, finalPath[finalPath.size() - 1].y / 20);
    changedNode->gCost = std::numeric_limits<float>::infinity();
    changedNode->walkable = false;

    for (int i = 0; i < 8; i++)
    {
        GridNode* neighbour = GetNeighbour(grid, changedNode, i);
        neighbour->parent = nullptr;

        int closedCount = CheckClosedList(neighbour);
        if (!neighbour || !neighbour->walkable || closedCount != -1) continue;
        //stop cutting corners
        int j = i;
        i == 7 ? j = -1 : j = i;
        GridNode* right = GetNeighbour(grid, changedNode, j + 1);
        GridNode* left = GetNeighbour(grid, changedNode, i - 1);
        if (i % 2 != 0) if (left && left->walkable == false || right && right->walkable == false) continue;
        
        neighboursVec.push_back(neighbour);
    }    



	while (current)
	{
		if (current == goal) 
		{
			//manually set the path up connected disjointed nodes
            AmmendPath(current);
			return true;
		}

        //go through each neighbour check if they exist in the changed neighbours 

		for (int i = 0; i < 8; i++)
		{
            GridNode* neighbour = GetNeighbour(grid, current, i);
            //neighbour->parent = nullptr;

            int bob = CheckChangedNeighbours(neighbour);
            if (bob == -1)
            {
                continue;
            }


            //TODO might be worth making theese checks a func
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
    if (!CheckConsistency(*grid.GetGridNode(finalPath[finalPath.size() - 1].x / 20, finalPath[finalPath.size() - 1].y / 20)))
    {        
        printf("nodes are inconsistant\n");
        if (finalPath.size() > 1)
        {
            bool check = RecalculatePath(grid, agent.GetPos(), Vector2(finalPath[finalPath.size() - 2].x, finalPath[finalPath.size() - 2].y));
            //if path is completely blocked search for new one
            if (!check) CreatePath(grid, agent.GetPos());
        }
        else
        {
            printf("Goal Gone Panic!!!\n");
            finalPath.clear();
        }
    }
    
    return true;
}
