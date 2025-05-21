#include "DynamicPathfinding.h"

#include "Dijkstra.h"
#include "AStar.h"
#include "Vector2.h"
#include "Agent.h"
#include "DStar.h"

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
			_dStar = new DStar();

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
			default:
				break;
			}			
			InputManager::HandleMouseClick(&e, _grid, *_agent, _nextNode);		
		}

		//imgui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();		

		
		static int mapIndex = 0;

		if (_imGuiWindow)
		{
			ImGui::Begin("Window", &_imGuiWindow);
			static std::string name;
			ImGui::InputText("Map Name", &name);
			if (ImGui::Button("Save"))
			{
				_grid->SaveCurrentGridLayout(name);
			}
			ImGui::Text("Load Map");
			ImGui::Combo("Map", &mapIndex, _mapPaths, IM_ARRAYSIZE(_mapPaths));
			if (ImGui::Button("Import Map"))
			{
				_grid->ResetGrid();
				_grid->ReGenerateGrid(_mapPaths[mapIndex]);
			}

			ImGui::Text("Algorithm Selector");
			ImGui::Combo("Algorithm", &_algIndex, _dropdownOptions, IM_ARRAYSIZE(_dropdownOptions));		

			if (ImGui::Button("Find Path"))
			{
				_takeStep = false;
				_grid->ResetGrid();
				switch (static_cast<Algorithm>(_algIndex))
				{
				case DIJKSTRA:
					if (_grid->goal)
					{
						_timeTaken = {};
						_clockStart = std::chrono::system_clock::now();
						_dijkstra->CreatePath(*_grid, _grid->WorldToGrid(_agent->GetPos()));
						_clockEnd = std::chrono::system_clock::now();
					}
					else printf("Goal Not Set");
					break;
				case ASTAR:
					if (_grid->goal)
					{
						_clockStart = std::chrono::system_clock::now();
						_aStar->CreatePath(*_grid, _grid->WorldToGrid(_agent->GetPos()));
						_clockEnd = std::chrono::system_clock::now();
					}
					else printf("Goal Not Set");
					break;
				case DSTARLITE:
					if (_grid->goal)
					{
						_clockStart = std::chrono::system_clock::now();
						printf("Player Pos: %f", _agent->GetPos().x);
						_dStar->CreatePath(*_grid, _agent->GetPos());
						_clockEnd = std::chrono::system_clock::now();
					}
					else printf("Goal Not Set");
					break;
				default:
					break;
				}
			}
			if (ImGui::Button("Take a Step"))
			{
				_takeStep = true;
				_grid->ResetStepGrid();
				switch (static_cast<Algorithm>(_algIndex))
				{
				case DIJKSTRA:
					if (_grid->goal)
					{
						_clockStart = std::chrono::system_clock::now();
						_dijkstra->CreatePath(*_grid, _grid->WorldToGrid(_agent->GetPos()));
						_clockEnd = std::chrono::system_clock::now();
					}
					else printf("Goal Not Set");
					break;
				case ASTAR:
					if (_grid->goal)
					{
						_clockStart = std::chrono::system_clock::now();
						_aStar->CreatePath(*_grid, _grid->WorldToGrid(_agent->GetPos()));
						_clockEnd = std::chrono::system_clock::now();
					}
					else printf("Goal Not Set");
					break;
				case DSTARLITE:
					if (_grid->goal)
					{
						_clockStart = std::chrono::system_clock::now();
						_dStar->CreatePath(*_grid, _grid->WorldToGrid(_agent->GetPos()));
						_clockEnd = std::chrono::system_clock::now();
						_nextNode = _dStar->finalPath[_dStar->finalPath.size() - 1];
					}
					else printf("Goal Not Set");
					break;
				default:
					break;
				}
			}
			static const char* stopButtonTxt = "Stop";
			if (ImGui::Button(stopButtonTxt))
			{
				isStopped = !isStopped;
				if (stopButtonTxt == "Stop")
					stopButtonTxt = "Start";
				else stopButtonTxt = "Stop";
			}
			if (ImGui::Button("Reset Path"))
			{
				_grid->ResetGrid();
			}

			_timeTaken = duration_cast<duration<float>>(_clockEnd - _clockStart).count();
			ImGui::Text("Time Taken: \n%f ms", (_timeTaken/60) * 1000);

			ImGui::End();
		}



		//clear Screen
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
		//doesnt clear paints over with the old colour
		SDL_RenderClear(_renderer);

		_grid->RenderGrid(_renderer);	

		bool reachedNode = false;

		if (!isStopped)
		{
			switch (static_cast<Algorithm>(_algIndex))
			{
			case DIJKSTRA:
				if (_dijkstra->finalPath.size() > 0)
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
				if (_dStar->finalPath.size() > 0)
				{
					if (_dStar->MoveForward(*_grid, *_agent))
					{
						if (_dStar->finalPath.size() == 0)
						{
							_nextNode = Vector2();
							break;
						}

						reachedNode = _agent->MoveToPosition(_dStar->finalPath[_dStar->finalPath.size() - 1], deltaTime);
						if (reachedNode)
						{
							_dStar->finalPath.pop_back();
							_count++;

							if (_dStar->finalPath.size() > 1)
								_nextNode = _dStar->finalPath[_dStar->finalPath.size() - 2];

							if (_takeStep && _count > 1)
							{
								_dStar->finalPath.clear();
								_count = 0;
							}
						}
					}
				}
				break;
			default:
				break;
			}
		}

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

	delete _grid;
	_grid 	   = nullptr;		
	delete _agent;
	_agent 	   = nullptr;
	delete _dijkstra;
	_dijkstra  = nullptr;
	delete _aStar;
	_aStar     = nullptr;
	delete _dStar;
	_dStar = nullptr;

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


