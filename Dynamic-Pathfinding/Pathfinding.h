#pragma once

#include <vector>
#include "Grid.h"
#include "Vector2.h"

class Pathfinding
{
protected:
	std::vector<GridNode*> openList;
	std::vector<GridNode*> closedList;

	int CheckOpenList(GridNode* node);
	int CheckClosedList(GridNode* node);
	virtual void SetPath(GridNode* node);
	GridNode* GetNeighbour(Grid& grid, GridNode* current, int dir);
public:
	Pathfinding();

	std::vector<Vector2> finalPath;
	virtual bool CreatePath(Grid& grid, Vector2 start) = 0;	

};

