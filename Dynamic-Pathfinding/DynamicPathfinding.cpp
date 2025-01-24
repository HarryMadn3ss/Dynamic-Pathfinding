#include "DynamicPathfinding.h"

#include "Dijkstra.h"
#include "AStar.h"
#include "Vector2.h"
#include "Agent.h"

#include <chrono>

using namespace std::chrono;

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
				printf("Renderer could not be created SDL Error: %s\n", SDL_GetError());
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
					printf("PNG loader failed to Initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				else
				{
					//gTexture = LoadTexture("Images/pika.png");
					// 
				}
			}

			//set up imgui
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		
			//set platform specs
			ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
			ImGui_ImplSDLRenderer2_Init(_renderer);

			_grid = new Grid;
			_agent = new Agent();
			_dijkstra = new Dijkstra();
			_aStar = new AStar();			

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
		static auto last = steady_clock::now();
		auto old = last;
		last = steady_clock::now();
		const duration<float> frameTime = last - old;
		float deltaTime = frameTime.count();

		//handles events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type)
			{
			case SDL_QUIT:
				_quit = true;
				break;
			case SDL_KEYDOWN:
				_quit = InputManager::HandleKeyInput(&e);
				break;
			case SDL_MOUSEBUTTONDOWN:
				InputManager::HandleMouseClick(&e, _grid);
			default:
				break;
			}
		}

		//imgui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();		

		if (_imGuiWindow)
		{
			ImGui::Begin("Window", &_imGuiWindow);
			static std::string name;
			ImGui::InputText("Map Name", &name);
			if (ImGui::Button("Save"))
			{
				_grid->SaveCurrentGridLayout(name);
			}
			static int algIndex = 0;
			ImGui::Text("Algorithm Selector");
			ImGui::Combo("Algorithm", &algIndex, _dropdownOptions, IM_ARRAYSIZE(_dropdownOptions));		

			if (ImGui::Button("Find Path"))
			{
				_takeStep = false;
				_grid->ResetGrid();
				switch (static_cast<Algorithm>(algIndex))
				{
				case DIJKSTRA:
					if (_grid->goal) _dijkstra->CreatePath(*_grid, _grid->WorldToGrid(_agent->GetPos()));
					else printf("Goal Not Set");
					break;
				case ASTAR:
					if (_grid->goal) _aStar->CreatePath(*_grid, _grid->WorldToGrid(_agent->GetPos()));
					else printf("Goal Not Set");
					break;
				case DSTARLITE:
					break;
				default:
					break;
				}
			}
			if (ImGui::Button("Take a Step"))
			{
				_takeStep = true;
				_grid->ResetStepGrid();
				switch (static_cast<Algorithm>(algIndex))
				{
				case DIJKSTRA:
					if (_grid->goal) _dijkstra->CreatePath(*_grid, _grid->WorldToGrid(_agent->GetPos()));
					else printf("Goal Not Set");
					break;
				case ASTAR:
					if (_grid->goal) _aStar->CreatePath(*_grid, _grid->WorldToGrid(_agent->GetPos()));
					else printf("Goal Not Set");
					break;
				case DSTARLITE:
					break;
				default:
					break;
				}
			}

			ImGui::End();
		}

		////blist = back buffer
		//SDL_BlitSurface(gImageToLoad, NULL, gScreenSurface, NULL);
		////switch buffers
		//SDL_UpdateWindowSurface(gWindow);

		//clear Screen
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
		//doesnt clear paints over with the old colour
		SDL_RenderClear(_renderer);

		//render texture to screen
		//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		//render red quad
		//SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		//SDL_SetRenderDrawColor(_renderer, 0xFF, 0x00, 0x00, 0xFF);
		//SDL_RenderFillRect(_renderer, &fillRect);

		_grid->RenderGrid(_renderer);	

		// if path.size() > 0
		// move to next node
		// remove from path
		bool reachedNode = false;

		switch (_selectedAlgorithm)
		{
		case DIJKSTRA:
			if(_dijkstra->finalPath.size() > 0)
			{
				reachedNode = _agent->MoveToPosition(_dijkstra->finalPath[0], deltaTime);
				if (reachedNode)
				{
					_dijkstra->finalPath.erase(_dijkstra->finalPath.begin());
					_count++;

					if (_takeStep && _count > 1)
					{					
						_dijkstra->finalPath.clear();
						_count = 0;						
					}
				}								
			}
			break;
		case ASTAR:
			if (_aStar->finalPath.size() > 0)
			{
				reachedNode = _agent->MoveToPosition(_aStar->finalPath[0], deltaTime);
				if (reachedNode)
				{
					_aStar->finalPath.erase(_aStar->finalPath.begin());
					_count++;

					if (_takeStep && _count > 1)
					{
						_aStar->finalPath.clear();
						_count = 0;						
					}
					
						
					
				}
			}
			break;
		case DSTARLITE:
			break;
		default:
			break;
		}
			
		//if (reachedNode == true)
		//{
		//	GridNode* node = _grid->GetGridNode(_grid->goal->position.x, _grid->goal->position.y);
		//	node->curentGoal = false;
		//	_grid->goal = nullptr;
		//	reachedNode = false;
		//}

		_agent->RenderAgent(_renderer);

		//imgui
		ImGui::Render();		
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), _renderer);		

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

	//ImGui
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui::DestroyContext();
}


