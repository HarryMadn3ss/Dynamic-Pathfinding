#pragma once
#include<math.h>

class Vector2
{
public:
	Vector2() { x = 0.0f; y = 0.0f; }
	Vector2(float _x, float _y) { x = _x; y = _y; }

	float x, y;	

	Vector2 operator+(Vector2 rhs){	return Vector2(x + rhs.x, y + rhs.y); }
	Vector2 operator+(int rhs) { return Vector2(x + 20, y + 20); }
	Vector2 operator-(Vector2 rhs) { return Vector2(x - rhs.x, y - rhs.y); }
	Vector2 operator* (float rhs) { return Vector2(x * rhs, y * rhs); }
	bool operator==(Vector2 rhs) { return (x == rhs.x) && (y == rhs.y); }
	bool operator!=(Vector2 rhs) { return (x != rhs.x) || (y != rhs.y); }


	float Magnitude();
	Vector2 Normalise();

};

