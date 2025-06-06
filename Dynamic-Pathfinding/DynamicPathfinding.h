#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

#include "commons.h"
#include "InputManager.h"
#include "Grid.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

class Dijkstra;
class Vector2;
class Agent;
class AStar;
class DStar;

class DynamicPathfinding
{
private:	
	SDL_Window*   _window = NULL;
	SDL_Surface*  _screenSurface = NULL;
	SDL_Surface*  _imageToLoad = NULL;
	SDL_Texture*  _texture = NULL;
	SDL_Renderer* _renderer = NULL;	
	Grid* _grid = NULL;
	Agent* _agent = NULL;
	Dijkstra* _dijkstra = NULL;
	AStar* _aStar = NULL;
	DStar* _dStar = NULL;

	const char* _mapPaths[5] = { "maps/MazeOne.txt", "maps/MazeTwo.txt", "maps/Decadence.txt", "maps/NoTalk.txt", "maps/Metro.txt" };

	std::chrono::time_point<std::chrono::system_clock> _clockStart, _clockEnd;
    float _timeTaken;
	bool _isStopped = false;
	bool _isErasing = false;
	bool _pathfound = false;
	int _colourChangeCount = 0;
	int _mutR = 0;
	int _mutG = 0;
	int _mutB = 0;

	const char* _dropdownOptions[3] = { "Dijkstra", "A*", "D* Lite" };
	int _algIndex = 0;
	Vector2 _nextNode = Vector2();
	bool _takeStep;
	int _count = 0;
	//ImGuiContext* _imGuiContext = NULL;
	bool _imGuiWindow = true;

	//main loop flag
	bool _quit = false;
protected:

public:
	//starts up and creates window
	bool Init();

	//game loop
	void GameLoop(SDL_Event& e);

	//free and close
	void Close();
};

