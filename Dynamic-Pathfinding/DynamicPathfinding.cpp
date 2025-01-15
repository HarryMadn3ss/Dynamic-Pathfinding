#include "DynamicPathfinding.h"


bool DynamicPathfinding::Init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else //create window
	{
		_window = SDL_CreateWindow("Dynamic Pathfinding", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (_window == NULL)
		{
			printf("Window could not be created! SDL_Erro: %s\n", SDL_GetError());
			success = false;
		}
		else
		{

			////////////////////////////////////////////////////////////////////////////////////PNG Loading/////////////////////////////////////////////////////////////////////////////
			//int imgFlags = IMG_INIT_PNG;
			//if (!(IMG_Init(imgFlags) & imgFlags))
			//{
			//	printf("PNG faild to Initialize! SDL_image Error: %s\n", IMG_GetError());
			//	success = false;
			//}
			//else
			//{
			//	//get window surface 
			//	gScreenSurface = SDL_GetWindowSurface(gWindow);

			//	SDL_Surface* imageToLoad = LoadSurface("Images/Panda.png");

			//	//to apply the stretch image
			//	SDL_Rect stretchRect;
			//	stretchRect.x = 0; //to make the size of screen pass in the screen dimensions
			//	stretchRect.y = 0;
			//	stretchRect.w = SCREEN_WIDTH;
			//	stretchRect.h = SCREEN_HEIGHT;
			//	SDL_BlitScaled(imageToLoad, NULL, gScreenSurface, &stretchRect);
			//	//fill surface with color
			//	//SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
			//}			

			///////////////////////////////////////////////////////////////////////////////////Texture Loading//////////////////////////////////////////////////////////////////////////
			//create renderer
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
			if (!_renderer)
			{
				printf("Renderer could not be created SDL Error: %s\n", SDL_GetError);
				success = false;
			}
			else
			{
				//renderer colour init
				SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//init PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("PNG faild to Initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				else
				{
					//gTexture = LoadTexture("Images/pika.png");
					// 

				}
			}

			//update surface
			SDL_UpdateWindowSurface(_window);
		}
	}

	return success;
}

void DynamicPathfinding::GameLoop(SDL_Event& e)
{
	while (!_quit)
	{


		//handles events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				_quit = true;
				break;
			case SDL_KEYDOWN:
				_quit = InputManager::HandleKeyInput(e);
				break;
			default:
				break;
			}
		}

		////blist = back buffer
		//SDL_BlitSurface(gImageToLoad, NULL, gScreenSurface, NULL);
		////switch buffers
		//SDL_UpdateWindowSurface(gWindow);

		//clear Screen
		SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		//doesnt clear paints over with the old colour
		SDL_RenderClear(_renderer);

		//render texture to screen
		//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		//render red quad
		SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		SDL_SetRenderDrawColor(_renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(_renderer, &fillRect);

		//update screen
		SDL_RenderPresent(_renderer);
	}	
}


void DynamicPathfinding::Close()
{
	//deallocate surface
	SDL_DestroyTexture(_texture);
	_texture = nullptr;
	SDL_FreeSurface(_imageToLoad);
	_imageToLoad = nullptr;

	//destroy window
	SDL_DestroyRenderer(_renderer);
	_renderer = nullptr;
	SDL_DestroyWindow(_window);
	_window = nullptr;

	//quit sdl subsystems
	IMG_Quit();
	SDL_Quit();
}


