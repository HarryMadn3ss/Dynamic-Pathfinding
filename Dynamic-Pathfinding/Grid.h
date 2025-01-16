#pragma once
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <string>


#include "commons.h"

//TODO create an enum for the different levels/maps/wahtever you wanna call it 

struct GridNode
{
	bool walkable = false;
};


class Grid
{
private:
	GridNode grid[GRID_WIDTH][GRID_HEIGHT];

	std::ifstream myFile;
protected:

public:
	Grid();
	~Grid();
	void GenerateGrid();
	void ReGenerateGrid(std::string path);

	void RenderGrid(SDL_Renderer* renderer);

	GridNode* GetGridNode(int x, int y);

	bool SaveCurrentGridLayout(std::string name);
};

