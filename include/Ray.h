#pragma once
#include "Program.h"
#include "Walls.h"

class Ray
{
public:
	Ray(const Vector2f& pBegining, const Vector2f& pEnd);
	Ray(const Vector2f& pBegining, const Vector2f& pEnd, 
		float pAngle, float pLength);

	void setBeginning(const Vector2f& pBegining);
	void setEnd(const Vector2f& pEnd);
	void setEnd(const Vector2f& pEnd, float pAngle, float pLength);

	Vector2f getBeginning() const noexcept;
	Vector2f getEnd() const noexcept;

	std::expected<std::vector<Vector2f>, bool> collide(const Walls& pWall);

	void render(SDL_Renderer* pRenderer) const;

private:
	Vector2f mBeginning{}, mEnd{};
	
};

