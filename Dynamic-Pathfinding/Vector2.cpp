#include "Vector2.h"

float Vector2::Magnitude()
{
    return sqrt(x * x + y * y);
}

Vector2 Vector2::Normalise()
{
    float mag = Vector2(x, y).Magnitude();
    return Vector2(x/mag, y/mag);
}
