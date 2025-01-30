#pragma once

#include "Pathfinding.h"

class DStar : public Pathfinding
{
private: 
	float ManhattenHueristic(GridNode current, GridNode goal);


public:
	bool CreatePath(Grid& grid, Vector2 start) override;
};

