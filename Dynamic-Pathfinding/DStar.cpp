#include "DStar.h"

float DStar::ManhattenHueristic(GridNode current, GridNode goal)
{
	Vector2 vec = goal.position - current.position;
	return abs(vec.x) + abs(vec.y);
}

bool DStar::CreatePath(Grid& grid, Vector2 start)
{
	return false;
}
