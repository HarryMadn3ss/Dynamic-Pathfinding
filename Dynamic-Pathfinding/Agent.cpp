#include "Agent.h"

Agent::Agent() : Agent(Vector2()) { }

Agent::Agent(Vector2 pos)
{
	position = pos;
	maxSpeed = 50.0f;
}

Agent::~Agent()
{
}

void Agent::RenderAgent(SDL_Renderer* renderer)
{
	SDL_Rect fillRect = { position.x, position.y, 15.0f, 15.0f };
	SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, &fillRect);
}

bool Agent::MoveToPosition(Vector2 goal, float dt)
{
	Vector2 dir = goal - position;
	float dist = dir.Magnitude();
	if (dist < 0.01) return true;
	dir = dir.Normalise();
	Vector2 velocity = dir * maxSpeed * dt;
	position = position + velocity;	
	//position = goal;
	return false;
}
