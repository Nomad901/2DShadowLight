#pragma once
struct Vector2f
{
	Vector2f& operator*(float pFactor) noexcept
	{
		x *= pFactor;
		y *= pFactor;
		return *this;
	}

	float getLength() const noexcept
	{
		return sqrt(x * x + y * y);
	}

	float getDistanceTo(const Vector2f pOther)
	{
		float dx = x - pOther.x;
		float dy = y - pOther.y;
		return sqrt(dx * dx + dy * dy);
	}

	float x, y;
};

