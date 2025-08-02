#pragma once
#include "Program.h"

class Arena
{
public:
	Arena(int32_t pWinWidth, int32_t pWinHeight);

	std::array<std::pair<Vector2f, Vector2f>,4> getWalls() const noexcept;

private:
	std::array<std::pair<Vector2f, Vector2f>, 4> mDimensions;
};

