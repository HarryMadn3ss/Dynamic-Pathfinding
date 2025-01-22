#pragma once
#include "Vector2.h"
#include <SDL.h>

class Agent
{
private:
	Vector2 position;
	float maxSpeed = 1.0f;
protected:

public:
	Agent();
	Agent(Vector2 pos);
	~Agent();

	void RenderAgent(SDL_Renderer* renderer);
	bool MoveToPosition(Vector2 goal);
	Vector2 GetPos() { return position; }
};

