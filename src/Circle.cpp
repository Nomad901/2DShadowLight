#include "Circle.h"

void Circle::init(int32_t pRadius, const Vector2f& pPos)
{
    mRadius = pRadius;
    mPos = pPos;
}

void Circle::setRadius(int32_t pRadius) noexcept
{
	mRadius = pRadius;
}

constexpr int32_t Circle::getRadius() noexcept
{
	return mRadius;
}

void Circle::setPos(Vector2f pPos) noexcept
{
	mPos = pPos;
}

Vector2f Circle::getPos() noexcept
{
	return mPos;
}

void Circle::setCircleFilled(bool pFilledCircle) noexcept
{
    mFilledCircle = pFilledCircle;
}

void Circle::setCircleColor(float pCircleColor[3]) noexcept
{
    mCircleColor = { pCircleColor[0], pCircleColor[1], pCircleColor[2]};
}

void Circle::setMultiplierSpeed(float pMultiplierSpeed) noexcept
{
    mMultiplierSpeed = pMultiplierSpeed;
}

std::unordered_map<SDL_Keycode, bool>& Circle::getStorageKeycodes()
{
    return mKeycodes;
}

void Circle::control(float pSpeed, float pArenaWidth, float pArenaHeight)
{
    float speed = pSpeed * mMultiplierSpeed;

    if (mKeycodes[SDLK_W])
        mPos.y -= speed;
    if (mKeycodes[SDLK_S])
        mPos.y += speed;
    if (mKeycodes[SDLK_D])
        mPos.x += speed;
    if (mKeycodes[SDLK_A])
        mPos.x -= speed;
    
    mPos.x = std::clamp(int32_t(mPos.x), 10, int32_t(pArenaWidth - 15));
    mPos.y = std::clamp(int32_t(mPos.y), 10, int32_t(pArenaHeight - 15));
}

void Circle::render(SDL_Renderer* pRenderer) const
{
    if (mFilledCircle)
    {
        circleRGBA(pRenderer, mPos.x, mPos.y,
                   mRadius,
                   (uint8_t)(mCircleColor[0] * 255),
                   (uint8_t)(mCircleColor[1] * 255),
                   (uint8_t)(mCircleColor[2] * 255),
                   255);
    }
    else
    {
        filledCircleRGBA(pRenderer, mPos.x, mPos.y,
                         mRadius,
                         (uint8_t)(mCircleColor[0] * 255),
                         (uint8_t)(mCircleColor[1] * 255),
                         (uint8_t)(mCircleColor[2] * 255),
                         255);
    }
}


