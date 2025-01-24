#pragma once

#include "Pathfinding.h"

class Dijkstra : public Pathfinding
{
public:

	bool CreatePath(Grid& grid, Vector2 start) override;		
};

