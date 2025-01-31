#pragma once

#include "Pathfinding.h"
#include "Agent.h"

class DStar : public Pathfinding
{
private: 
	float ManhattenHueristic(GridNode current, GridNode goal);

	GridNode* GetCheapestNode();
	//void SetPath(GridNode* end) override;
	bool CheckConsistency(GridNode& node);
	float GetTravelCost(int dir);
public:
	bool CreatePath(Grid& grid, Vector2 start);
	bool CreatePath(Grid& grid, Vector2 start, Vector2 _goal);
	bool MoveForward(Grid& grid, Agent& agent);
};

