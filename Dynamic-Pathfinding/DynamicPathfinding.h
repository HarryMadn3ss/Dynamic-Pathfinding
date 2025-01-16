#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>

#include "commons.h"
#include "InputManager.h"
#include "Grid.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

class DynamicPathfinding
{
private:	
	SDL_Window*   _window = NULL;
	SDL_Surface*  _screenSurface = NULL;
	SDL_Surface*  _imageToLoad = NULL;
	SDL_Texture*  _texture = NULL;
	SDL_Renderer* _renderer = NULL;	
	Grid* _grid = NULL;

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

