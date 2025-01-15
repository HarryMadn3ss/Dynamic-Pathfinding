#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>

#include "commons.h"


//globals
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gImageToLoad = NULL;
SDL_Texture* gTexture = NULL;
SDL_Renderer* gRenderer = NULL;

//main loop flag
bool gQuit = false;

//starts up and creates window
bool Init();

//load media
bool LoadMedia();

//
SDL_Surface* LoadSurface(std::string path);

//load image as texture
SDL_Texture* LoadTexture(std::string path);

//handle key input
void HandleKeyInput(SDL_Event& e);

//free and close
void Close();

int main(int argc, char* args[])
{
	if (!Init())
	{
		printf("Failed to Init: SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		//if (!LoadMedia())
		//{
		//	printf("Faild to load media: SDL_Error: %s\n", SDL_GetError());
		//}
	}

	//hack to get window to stay up XD
	//event handler
	SDL_Event e; 
	

	while (!gQuit) 
	{ 
		//handles events on queue
		while (SDL_PollEvent(&e) != 0)
		{ 
			switch (e.type)
			{
			case SDL_QUIT:
				gQuit = true;
				break;
			case SDL_KEYDOWN:
				HandleKeyInput(e);
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

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//render texture to screen
		//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		//render red quad
		SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, &fillRect);


		//update screen
		SDL_RenderPresent(gRenderer);
	}

	Close();


	return 0;
}

bool Init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{ 
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError()); 
		success = false;
	}
	else //create window
	{
		gWindow = SDL_CreateWindow("Dynamic Pathfinding", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) 
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
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (!gRenderer)
			{
				printf("Renderer could not be created SDL Error: %s\n", SDL_GetError);
				success = false;
			}
			else
			{
				//renderer colour init
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

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
			SDL_UpdateWindowSurface(gWindow);			
		}
	}

	return success;
}

bool LoadMedia()
{
	bool success = true;

	//load PNG
	gTexture = LoadTexture("Images/pika.png");
	if (gTexture == NULL)
	{
		printf("Texture image failed to load: SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	return success;
}

SDL_Surface* LoadSurface(std::string path)
{
	//the final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//load image from path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (!loadedSurface) printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	else
	{
		//convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (!optimizedSurface)
		{
			printf("Surface Faild to optimize SDL Error: %s\n", SDL_GetError());
		}

		//free the old surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

SDL_Texture* LoadTexture(std::string path)
{
	//final texure
	SDL_Texture* newTex = NULL;

	//load image from path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (!loadedSurface) printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	else
	{
		//convert surface to screen format
		newTex = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (!newTex)
		{
			printf("Surface Failed to create texture SDL Error: %s\n", SDL_GetError());
		}

		//free the old surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTex;
}

void HandleKeyInput(SDL_Event& e)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		gQuit = true;
		break;
	default:
		break;
	}
}

void Close()
{
	//deallocate surface
	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;
	SDL_FreeSurface(gImageToLoad);
	gImageToLoad = nullptr;

	//destroy window
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	//quit sdl subsystems
	IMG_Quit();
	SDL_Quit();
}
