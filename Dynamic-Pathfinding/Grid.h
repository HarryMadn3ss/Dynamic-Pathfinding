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
	
	//TODO break out into two diffent structs this is getting messy
	float rhsCost, dStarGCost;

	GridNode* parent = nullptr;
	GridNode* node = nullptr;

	bool curentGoal = false;
	bool inPath = false;
	bool searched = false;
	bool updatedPath = false;

	GridNode()
	{
		position = Vector2();
		walkable = false;
		gCost = 0.0f;
		fCost = 0.0f;
		hCost = 0.0f;
		rhsCost = std::numeric_limits<float>::infinity();
		dStarGCost = -1.0f;
	};
	GridNode(GridNode* _node, GridNode* _parent, float _cost)
	{
		node = _node;
		parent = _parent;
		fCost = _cost;
		gCost = 0.0f;
		hCost = 0.0f;
		if (node->walkable) rhsCost = 0.0f;
		else rhsCost = std::numeric_limits<float>::infinity();
		dStarGCost = -1.0f;
	}

	bool IsConsistant() const
	{
		return gCost == rhsCost;
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

	void RenderGrid(SDL_Renderer* renderer, int colourChangeCount, int mutR, int mutG, int mutB);

	GridNode* GetGridNode(int x, int y);

	bool SaveCurrentGridLayout(std::string name);

	GridNode* goal = NULL;

	Vector2 WorldToGrid(Vector2 pos) { return Vector2(static_cast<int>(pos.x) / 20 , static_cast<int>(pos.y) / 20  ); }
	void ResetGrid();
	void ResetStepGrid();


};

