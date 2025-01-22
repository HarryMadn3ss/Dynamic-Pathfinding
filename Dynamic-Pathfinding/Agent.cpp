#include "Agent.h"

Agent::Agent()
{
	position = Vector2();
	maxSpeed = 1.0f;
}

Agent::Agent(Vector2 pos)
{
	position = pos;
	maxSpeed = 1.0f;
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

bool Agent::MoveToPosition(Vector2 goal)
{
	while (goal != position)
	{
		Vector2 dir = goal - position;
		dir = dir.Normalise();
		Vector2 velocity = dir * maxSpeed;
		position = position + velocity;
	}
	return true;
}
