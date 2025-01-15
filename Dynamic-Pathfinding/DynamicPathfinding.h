#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>

#include "commons.h"
#include "InputManager.h"


class DynamicPathfinding
{
private:	
	SDL_Window*   _window = NULL;
	SDL_Surface*  _screenSurface = NULL;
	SDL_Surface*  _imageToLoad = NULL;
	SDL_Texture*  _texture = NULL;
	SDL_Renderer* _renderer = NULL;	

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

