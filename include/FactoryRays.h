#pragma once
#include "Program.h"
#include "Ray.h"

class FactoryRays
{
public:
	FactoryRays();
	FactoryRays(const Vector2f& pBeginning, const Vector2f& pEnd);
	FactoryRays(const Vector2f& pBeginning, const Vector2f& pEnd, 
				float pAngle, float pLength);
	
	void pushRay(const Vector2f& pBeginning, const Vector2f& pEnd);
	void pushRay(const Vector2f& pBeginning, const Vector2f& pEnd, 
				 float pAngle, float pLength);
	void pushRays(std::initializer_list<std::pair<Vector2f, Vector2f>> pRays,
				  float pLength);
	void pushRays(const Vector2f& pBeginning, const Vector2f& pEnd, 
				  int32_t pNumber, float pLength);
	void eraseRay(size_t pIdentity);
	
	int32_t getNumber() const noexcept;
	float getLastLength() const noexcept;
	float getLastAngle() const noexcept;

	Vector2f getBeginningRay(size_t pIdentity) const noexcept;
	Vector2f getEndRay(size_t pIdentity) const noexcept;

	void update(size_t pIdentity, 
				const Vector2f& pBeginning, const Vector2f& pEnd);
	void update(const Vector2f& pBeginning, const Vector2f& pEnd, 
				bool pPreserveRays = false);
	void update(const Vector2f& pBeginning);

	void render(SDL_Renderer* pRenderer) const noexcept;
	void render(SDL_Renderer* pRenderer, Walls pWall);

private:
	std::vector<Ray> mRayStorage;

	float mLength{};
	float mAngle{};
};



//Resources::Vector2f pos = { 500, 400 };
//mStrgRays.reserve(40);
//for (int32_t i = 0; i < mStrgRays.capacity(); i++)
//{
//	Ray newRay = { Resources::sVector2f, {500,400}, float(float(i) * 3.14 / M_PI) };
//	mStrgRays.emplace_back(std::move(newRay));
//}