#include "Ray.h"

Ray::Ray(const Vector2f& pBegining, const Vector2f& pEnd)
{
	mBeginning = pBegining;
	mEnd = pEnd;
}

Ray::Ray(const Vector2f& pBegining, const Vector2f& pEnd,
		 float pAngle, float pLength)
{
	mBeginning = pBegining;
	
	mEnd.x = pEnd.x + pLength * cosf(pAngle);
	mEnd.y = pEnd.y + pLength * sinf(pAngle);
}

void Ray::setBeginning(const Vector2f& pBegining)
{
	mBeginning = pBegining;
}

void Ray::setEnd(const Vector2f& pEnd)
{
	mEnd = pEnd;
}

void Ray::setEnd(const Vector2f& pEnd, float pAngle, float pLength)
{
	mEnd.x = pEnd.x + pLength * cosf(pAngle);
	mEnd.y = pEnd.y + pLength * sinf(pAngle);
}

Vector2f Ray::getBeginning() const noexcept
{
	return mBeginning;
}

Vector2f Ray::getEnd() const noexcept
{
	return mEnd;
}

std::expected<std::vector<Vector2f>, bool> Ray::collide(const Walls& pWall)
{
    static float mT = 0.0f;
    static float mU = 0.0f;
    std::vector<Vector2f> tmpVctr; 
    tmpVctr.reserve(pWall.getWallsStrg().size());

    for (auto& i : pWall.getWallsStrg())
    {
        const float x1 = i.first.x;
        const float y1 = i.first.y;
        const float x2 = i.second.x;
        const float y2 = i.second.y;

        const float x3 = mBeginning.x;
        const float y3 = mBeginning.y;
        const float x4 = mEnd.x;
        const float y4 = mEnd.y;

        float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (den == 0)
            return std::unexpected(false);

        mT = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
        mU = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
        if (mT > 0 && mT < 1 && mU > 0)
        {
            Vector2f pointVctr = { x1 + mT * (x2 - x1),
                                   y1 + mT * (y2 - y1) };
            tmpVctr.push_back(pointVctr);
        }
        else
            return std::unexpected(false);
    }
    return tmpVctr;
}

void Ray::render(SDL_Renderer* pRenderer) const
{
    SDL_RenderLine(pRenderer, mBeginning.x, mBeginning.y, mEnd.x, mEnd.y);
}
