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
	std::string path = "maps/map.txt";
	myFile.open(path);
	if (!myFile.good()) printf("Map file failed to open: path: %s\n", path);
	else
	{
		if (myFile.is_open())
		{
			std::string fileReadIn;
			//myFile >> fileReadIn;
						
			int stringCount = 0;
			while (getline(myFile, fileReadIn))
			{
				for (int j = 0; j < GRID_WIDTH; j++)
				{					
					grid[j][stringCount].walkable = fileReadIn[j] - '0';				
				}	
				stringCount++;
			}
		}
	}
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
				grid[j][i].walkable = 1;
			}
			else grid[j][i].walkable = 0;
		}
	}
}

void Grid::ReGenerateGrid(std::string path)
{
	myFile.open(path);
	if (!myFile.good()) printf("Map file failed to open: path: %s\n", path);
	else
	{
		if (myFile.is_open())
		{
			std::string fileReadIn;
			//myFile >> fileReadIn;

			int stringCount = 0;
			while (getline(myFile, fileReadIn))
			{
				for (int j = 0; j < GRID_WIDTH; j++)
				{
					grid[j][stringCount].walkable = fileReadIn[j] - '0';
				}
				stringCount++;
			}
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
			if (grid[j][i].walkable == 1) 
			{
				SDL_Rect outlineRect = { j * 20, i * 20, 20.0f, 20.0f };
				SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xff);
				SDL_RenderDrawRect(renderer, &outlineRect);				
			}
			else
			{
				SDL_Rect fillRect = { j * 20, i * 20, 20.0f, 20.0f };
				SDL_SetRenderDrawColor(renderer, 0xFF + j*i, 0x00 + j*j, 0x00 + i*i, 0xFF);
				SDL_RenderFillRect(renderer, &fillRect);
			}
		}
	}
}

GridNode* Grid::GetGridNode(int x, int y)
{
	return &grid[x][y];
}
