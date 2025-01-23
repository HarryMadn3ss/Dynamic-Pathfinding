#pragma once
#include <vector>

#include "Grid.h"

class AStar
{
private:
	std::vector<GridNode*> openList;
	std::vector<GridNode*> closedList;

	int CheckClosedList(GridNode* node);
	int CheckOpenList(GridNode* node);

	void SetPath(GridNode* end);

	GridNode* GetNeighbour(Grid& grid, GridNode* current, int dir);
	float ManhattenHeuristic(GridNode* current, GridNode* goal);
	GridNode* GetCheapestNode();
protected:

public:
	AStar();

	std::vector<Vector2> finalPath;
	bool CreatePath(Grid& grid, Vector2 start);
};

