#pragma once

#include "Pathfinding.h"
#include "Agent.h"

class DStar : public Pathfinding
{
private: 
	std::vector<GridNode*> neighboursVec;

	int CheckChangedNeighbours(GridNode* node);
	void AmmendPath(GridNode* node);

	float ManhattenHueristic(GridNode current, GridNode goal);

	GridNode* GetCheapestNode();
	//void SetPath(GridNode* end) override;
	bool CheckConsistency(GridNode& node);
	float GetTravelCost(int dir);

	int count = 0;
public:
	bool CreatePath(Grid& grid, Vector2 start);
	bool RecalculatePath(Grid& grid, Vector2 start, Vector2 _goal);
	bool MoveForward(Grid& grid, Agent& agent);
};

