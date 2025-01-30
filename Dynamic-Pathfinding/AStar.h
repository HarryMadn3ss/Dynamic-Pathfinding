#pragma once
#include "Pathfinding.h"

class AStar : public Pathfinding
{
private:
	float ManhattenHeuristic(GridNode current, GridNode goal);
	GridNode* GetCheapestNode();
public:
	bool CreatePath(Grid& grid, Vector2 start) override;
};

