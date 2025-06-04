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
	std::string path = "maps/Empty.txt";
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
					grid[j][stringCount].position = Vector2(j, stringCount);					
					grid[j][stringCount].gCost = 1.0f;
					grid[j][stringCount].hCost = 0.0f;	
					grid[j][stringCount].fCost = 0.0f;	
					grid[j][stringCount].parent = nullptr;
					grid[j][stringCount].node = nullptr;
					grid[j][stringCount].curentGoal = false;
					grid[j][stringCount].inPath = false;
					grid[j][stringCount].searched = false;
				}	
				stringCount++;
			}
		}
	}
	myFile.close();
}

Grid::~Grid()
{
	if(myFile.is_open()) myFile.close();	
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
				grid[j][i].gCost = 1;
			}
			else
			{
				grid[j][i].walkable = 0;
				grid[j][i].gCost = std::numeric_limits<float>::infinity();
			}
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
	myFile.close();
}

void Grid::RenderGrid(SDL_Renderer* renderer)
{
	//TODO: needs a refactor soon
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (grid[j][i].curentGoal)
			{
				SDL_Rect outlineRect = { j * 20, i * 20, 20.0f, 20.0f };
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xff);
				SDL_RenderFillRect(renderer, &outlineRect);
				continue;
			}
			if (grid[j][i].inPath)
			{
				//SDL_Rect rect = { j * 20, i * 20, 20.0f, 20.0f };
				//SDL_SetRenderDrawColor(renderer, 0xFF, 0xAA, 0x00, 0xff);
				//SDL_RenderFillRect(renderer, &rect);
				//continue;
				SDL_Rect rect = { j * 20, i * 20, 20.0f, 20.0f };
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xAA, 0x00, 0xff);
				SDL_RenderDrawRect(renderer, &rect);
				continue;
			}
			if (grid[j][i].searched)
			{
				SDL_Rect rect = { j * 20, i * 20, 20.0f, 20.0f };
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xAA, 0x00, 0xff);
				SDL_RenderDrawRect(renderer, &rect);
				continue;
			}
			//can change the colours/fill amount based upon walkable/notwalkable
			if (grid[j][i].walkable == 1) 
			{
				SDL_Rect outlineRect = { j * 20, i * 20, 20.0f, 20.0f };
				SDL_SetRenderDrawColor(renderer, 0xaa, 0xFF, 0xaa, 0xff);
				SDL_RenderDrawRect(renderer, &outlineRect);				
			}
			else
			{
				SDL_Rect fillRect = { j * 20, i * 20, 20.0f, 20.0f };
				SDL_SetRenderDrawColor(renderer, 0xaa, 0xff, 0xaa, 0xff);
				SDL_RenderFillRect(renderer, &fillRect);
			}
		}
	}
}

GridNode* Grid::GetGridNode(int x, int y)
{
	if (x >= GRID_WIDTH || y >= GRID_HEIGHT || x < 0 || y < 0) return nullptr;
	return &grid[x][y];
}

bool Grid::SaveCurrentGridLayout(std::string name)
{
	std::string pathName = "maps/" + name + ".txt";

	std::ofstream file(pathName);
	if (!file.good()) return false;

	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			file << grid[j][i].walkable;
		}
		file << "\n";
	}
	file.close();
	return true;
}

void Grid::ResetGrid()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			grid[j][i].inPath = false;
			grid[j][i].searched = false;
			grid[j][i].curentGoal = false;
			grid[j][i].parent = nullptr;
			grid[j][i].gCost = 1.0f;
			grid[j][i].hCost = 0.0f;
			grid[j][i].fCost = 0.0f;
		}
	}
}

void Grid::ResetStepGrid()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			grid[j][i].inPath = false;			
			grid[j][i].parent = nullptr;
			grid[j][i].gCost = 1.0f;
			grid[j][i].hCost = 0.0f;
			grid[j][i].fCost = 0.0f;
		}
	}
}
