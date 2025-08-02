#pragma once
struct Vector2f
{
	float x, y;
	
	float getDistanceTo(const Vector2f pOther)
	{
		float dx = x - pOther.x;
		float dy = y - pOther.y;
		return sqrt(dx * dx + dy * dy);
	}
};

