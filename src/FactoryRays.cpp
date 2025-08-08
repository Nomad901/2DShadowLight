#include "FactoryRays.h"

FactoryRays::FactoryRays()
{
	mRayStorage.reserve(30);
}

FactoryRays::FactoryRays(const Vector2f& pBeginning, const Vector2f& pEnd)
{
	mRayStorage.reserve(30);
	mRayStorage.push_back({ pBeginning, pEnd });
}

FactoryRays::FactoryRays(const Vector2f& pBeginning, const Vector2f& pEnd, float pAngle, float pLength)
{
	pAngle = mAngle;
	mLength = pLength;

	mRayStorage.reserve(30);
	Vector2f tmpEnd = pEnd;
	tmpEnd.x = pEnd.x + pLength * cosf(pAngle);
	tmpEnd.y = pEnd.y + pLength * sinf(pAngle);
	mRayStorage.push_back({ pBeginning, tmpEnd });
}

void FactoryRays::pushRay(const Vector2f& pBeginning, const Vector2f& pEnd)
{
	mRayStorage.push_back({ pBeginning, pEnd });
}

void FactoryRays::pushRay(const Vector2f& pBeginning, const Vector2f& pEnd, float pAngle, float pLength)
{
	mLength = pLength;
	mAngle = pAngle;
	Vector2f tmpEnd = pEnd;
	tmpEnd.x = pEnd.x + pLength * cosf(pAngle);
	tmpEnd.y = pEnd.y + pLength * sinf(pAngle);
	mRayStorage.push_back({ pBeginning, tmpEnd });
}

void FactoryRays::pushRays(std::initializer_list<std::pair<Vector2f, Vector2f>> pRays, 
						   float pLength)
{
	mLength = pLength;
	int32_t counter = 1;
	for (auto& i : pRays)
	{
		pushRay(i.first, i.second, counter, pLength);
		counter++;
	}
}

void FactoryRays::pushRays(const Vector2f& pBeginning, const Vector2f& pEnd, 
						   int32_t pNumber, float pLength)
{
	mNumber = pNumber;
	mLength = pLength;
	for (int32_t i = 0; i < pNumber; i++)
	{
		pushRay(pBeginning, pEnd, i, pLength);
	}
}

void FactoryRays::eraseRay(size_t pIdentity)
{
	assert(pIdentity < mRayStorage.size());

	mRayStorage.erase(mRayStorage.begin() + pIdentity);
}

void FactoryRays::setNumberRays(int32_t pNumber)
{
	mNumber = pNumber;
}

void FactoryRays::setLength(float pLength)
{
	mLength = pLength;
}

int32_t FactoryRays::getNumber() const noexcept
{
	return std::ssize(mRayStorage);
}

float FactoryRays::getLastLength() const noexcept
{
	return mLength;
}

float FactoryRays::getLastAngle() const noexcept
{
	return mAngle;
}

const std::vector<Ray>& FactoryRays::getRayStorage() const noexcept
{
	return mRayStorage;
}

Vector2f FactoryRays::getBeginningRay(size_t pIdentity) const noexcept
{
	assert(pIdentity < mRayStorage.size());
	return mRayStorage[pIdentity].getBeginning();
}

Vector2f FactoryRays::getEndRay(size_t pIdentity) const noexcept
{
	assert(pIdentity < mRayStorage.size());
	return mRayStorage[pIdentity].getEnd();
}

void FactoryRays::update(size_t pIdentity, const Vector2f& pBeginning, const Vector2f& pEnd)
{
	assert(pIdentity < mRayStorage.size());
	mRayStorage[pIdentity].setBeginning(pBeginning);
	mRayStorage[pIdentity].setEnd(pEnd);
}

void FactoryRays::update(const Vector2f& pBeginning, const Vector2f& pEnd,
						 bool pPreserveRays)
{
	for (auto& i : mRayStorage)
	{
		if (!pPreserveRays)
		{
			i.setBeginning(pBeginning);
			i.setEnd(pEnd);
		}
		else
		{
			mRayStorage.clear();
			pushRays(pBeginning, pEnd, mNumber, mLength);
		}
	}
}

void FactoryRays::update(const Vector2f& pBeginning)
{
	for (auto& i : mRayStorage)
	{
		i.setBeginning(pBeginning);
	}
}

void FactoryRays::render(SDL_Renderer* pRenderer) const noexcept
{
	for (auto& i : mRayStorage)
	{
		Vector2f beginning = i.getBeginning();
		Vector2f end = i.getEnd();

		SDL_RenderLine(pRenderer, beginning.x, beginning.y, end.x, end.y);
	}
}

void FactoryRays::render(SDL_Renderer* pRenderer, Walls pWall)
{
	for (auto& ray : mRayStorage)
	{
		Vector2f closest = { -100.0f,-100.0f };
		float record = std::numeric_limits<float>::infinity();
		for (auto& wall : pWall.getWallsStrg())
		{
			auto value = ray.collide(wall);
			if (value.has_value())
			{
				float distance = ray.getBeginning().getDistanceTo(value.value()[0]);
				if (distance < record)
				{
					record = distance;
					closest = value.value()[0];
				}
			}
		}
		if (record < std::numeric_limits<float>::infinity())
		{
			SDL_RenderLine(pRenderer, ray.getBeginning().x, ray.getBeginning().y, closest.x, closest.y);
		}
	}
}
