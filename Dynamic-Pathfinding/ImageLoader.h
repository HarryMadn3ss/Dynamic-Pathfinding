#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>


namespace ImageLoader
{
	//load media
	//bool LoadMedia(SDL_Texture* tex);

	//image to surface load
	SDL_Surface* LoadSurfaceImage(std::string path, SDL_Surface* surface);

	//load image as texture
	SDL_Texture* LoadTexture(std::string path, SDL_Renderer* renderer);
};

