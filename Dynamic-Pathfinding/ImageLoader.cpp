#include "ImageLoader.h"


//bool ImageLoader::LoadMedia(SDL_Texture* tex)
//{
//	bool success = true;
//
//	//load PNG
//	tex = LoadTexture("Images/pika.png", nullptr);
//	if (tex == NULL)
//	{
//		printf("Texture image failed to load: SDL Error: %s\n", SDL_GetError());
//		success = false;
//	}
//	return success;
//}

SDL_Surface* ImageLoader::LoadSurfaceImage(std::string path, SDL_Surface* surface)
{
	//the final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//load image from path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (!loadedSurface) printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	else
	{
		//convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, surface->format, 0);
		if (!optimizedSurface)
		{
			printf("Surface Faild to optimize SDL Error: %s\n", SDL_GetError());
		}

		//free the old surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

SDL_Texture* ImageLoader::LoadTexture(std::string path, SDL_Renderer* renderer)
{
	//final texure
	SDL_Texture* newTex = NULL;

	//load image from path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (!loadedSurface) printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	else
	{
		//convert surface to screen format
		newTex = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (!newTex)
		{
			printf("Surface Failed to create texture SDL Error: %s\n", SDL_GetError());
		}

		//free the old surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTex;
}
