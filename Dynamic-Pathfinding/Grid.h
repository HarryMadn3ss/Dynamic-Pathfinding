#pragma once
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Vector2.h"

#include "commons.h"

//TODO create an enum for the different levels/maps/wahtever you wanna call it 

struct GridNode
{
	Vector2 position;
	bool walkable = false;
	float gCost, hCost, fCost;

	GridNode* parent = nullptr;
	GridNode* node = nullptr;

	bool curentGoal = false;

	GridNode()
	{
		position = Vector2();
		walkable = false;
		gCost = 0.0f;
		fCost = 0.0f;
		hCost = 0.0f;
	};
	GridNode(GridNode* node, GridNode* _parent, float _cost)
	{
		node = node;
		parent = _parent;
		fCost = _cost;
	}
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

	GridNode* goal = NULL;
};

