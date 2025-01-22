#pragma once
#include <vector>

#include "Grid.h"

class Dijkstra
{
private:

	std::vector<GridNode*> openList;
	std::vector<GridNode*> closedList;
		
	int CheckClosedList(GridNode* node);
	int CheckOpenList(GridNode* node);
	void SetPath(GridNode* end);

	GridNode* GetNeighbour(Grid& grid, GridNode* current, int dir);
public:
	Dijkstra();

	std::vector<Vector2> finalPath;
	bool CreatePath(Grid& grid, Vector2 start, Vector2 goal);
	
	
};

