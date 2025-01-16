#pragma once
#include "commons.h"
#include <SDL.h>

class Grid
{
private:
	int grid[GRID_WIDTH][GRID_HEIGHT];

protected:

public:
	Grid();
	~Grid();
	void GenerateGrid();

	void RenderGrid(SDL_Renderer* renderer);
};

