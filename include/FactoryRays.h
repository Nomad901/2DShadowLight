#pragma once
#include <random>

#include "Program.h"
#include "Ray.h"
#include "glm.hpp"

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
	
	void setNumberRays(int32_t pNumber);
	void setLength(float pLength);
	int32_t getNumberRays() const noexcept;
	float getLength() const noexcept;
	int32_t getNumber() const noexcept;
	float getLastLength() const noexcept;
	float getLastAngle() const noexcept;
	const std::vector<Ray>& getRayStorage() const noexcept;

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

	int32_t mNumber{ 50 };

	float mLength{};
	float mAngle{};
};


