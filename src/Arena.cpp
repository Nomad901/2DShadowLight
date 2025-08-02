#include "Arena.h"

Arena::Arena(int32_t pWinWidth, int32_t pWinHeight)
{
    std::pair<Vector2f, Vector2f> leftUp = { { 10.0f, 10.0f }, { float(pWinWidth) - 50.0f, 10.0f } };
    std::pair<Vector2f, Vector2f> rightUp = { {10.0f,10.0f}, {10.0f, float(pWinHeight) - 50.0f} };
    std::pair<Vector2f, Vector2f> rightDown = { {float(pWinWidth) - 50.0f, 10.0f}, {float(pWinWidth) - 50.0f,float(pWinHeight) - 50.0f} };
    std::pair<Vector2f, Vector2f> leftDown = { {10.0f, float(pWinHeight) - 50.0f}, {float(pWinWidth) - 50.0f,float(pWinHeight) - 50.0f} };

    mDimensions =
    {
        leftUp,
        rightUp,
        rightDown,
        leftDown
    };
}

std::array<std::pair<Vector2f, Vector2f>, 4> Arena::getWalls() const noexcept
{
    return mDimensions;
}
