#include "DynamicPathfinding.h"




int main(int argc, char* args[])
{
	DynamicPathfinding* origin = new DynamicPathfinding;

	if (!origin->Init())
	{
		printf("Failed to Init: SDL Error: %s\n", SDL_GetError());
	}

	//event handler
	SDL_Event e; 	

	origin->GameLoop(e);

	origin->Close();

	return 0;
}


