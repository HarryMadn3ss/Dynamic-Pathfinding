#include "Grid.h"

Grid::Grid()
{
	//for (int i = 0; i < SCREEN_HEIGHT; i++)
	//{
	//	for (int j = 0; j < SCREEN_WIDTH; j++)
	//	{
	//		if (j % 2 == 0)
	//		{
	//			grid[j][i] = 1;
	//		}
	//		else grid[j][i] = 0;
	//	}
	//}
}

Grid::~Grid()
{
	delete[] grid;
}

void Grid::GenerateGrid()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (j % 2 == 0)
			{
				grid[j][i] = 1;
			}
			else grid[j][i] = 0;
		}
	}
}

void Grid::RenderGrid(SDL_Renderer* renderer)
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			//can change the colours/fill amount based upon walkable/notwalkable
			if (grid[j][i] == 1) 
			{
				SDL_Rect fillRect = { j * 20, i * 20, 20.0f, 20.0f };
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &fillRect);
			}
			else
			{
				SDL_Rect outlineRect = { j * 20, i * 20, 20.0f, 20.0f };
				SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xff);
				SDL_RenderDrawRect(renderer, &outlineRect);
			}
		}
	}
}
