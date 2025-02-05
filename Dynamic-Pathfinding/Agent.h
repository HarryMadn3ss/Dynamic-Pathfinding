#pragma once
#include "Vector2.h"
#include <SDL.h>

class Agent
{
private:
	Vector2 position;
	float maxSpeed;
protected:

public:
	Agent();
	Agent(Vector2 pos);
	~Agent();

	void RenderAgent(SDL_Renderer* renderer);
	bool MoveToPosition(Vector2 goal, float dt);
	Vector2 GetPos() { return position; }
	void SetPos(Vector2 pos) { position = pos; }
};

