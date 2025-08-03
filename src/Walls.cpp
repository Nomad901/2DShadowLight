#include "Walls.h"

Walls::Walls()
{
	mWalls.reserve(20);
}

Walls::Walls(const std::pair<Vector2f, Vector2f>& pPosWall)
{
	mWalls.reserve(20);
	mWalls.emplace_back(pPosWall);
}

Walls::Walls(std::span<std::pair<Vector2f, Vector2f>> pPosWalls)
{
	mWalls.reserve(20);
	for (auto& i : pPosWalls)
	{
		mWalls.emplace_back(i);
	}
}

auto Walls::getWallsStrg() const noexcept -> std::vector<std::pair<Vector2f, Vector2f>>
{
	return mWalls;
}

auto Walls::createWallAroundBlock(std::vector<SDL_FRect>& pBlocks, size_t pId) -> void
{
	assert(pId < pBlocks.size());

	mWalls.clear();
	
	for (auto& i : mArena)
	{
		mWalls.push_back(i);
	}

	auto sidesOccupied = checkForBlockFromSide(pBlocks, pId);
	auto hasSide = [sidesOccupied](Side pSide) -> bool
		{
			return std::find(sidesOccupied.begin(), sidesOccupied.end(), pSide) != sidesOccupied.end();
		};
	
	SDL_FRect block = pBlocks[pId];

	if (!hasSide(Side::RIGHT))
		mWalls.push_back(std::make_pair<Vector2f, Vector2f>({ block.x + block.w, block.y },
															{ block.x + block.w, block.y + block.h }));
	if (!hasSide(Side::LEFT))
		mWalls.push_back(std::make_pair<Vector2f, Vector2f>({ block.x, block.y }, 
															{ block.x, block.y+block.h }));
	if (!hasSide(Side::UP))
		mWalls.push_back(std::make_pair<Vector2f, Vector2f>({ block.x, block.y },
															{ block.x + block.w, block.y }));
	if (!hasSide(Side::DOWN))
		mWalls.push_back(std::make_pair<Vector2f, Vector2f>({ block.x, block.y + block.h },
															{ block.x + block.w, block.y + block.h }));
}

auto Walls::createWallsFromBlocks(std::vector<SDL_FRect>& pBlocks) -> void
{
	mWalls.clear();

	for (auto& i : mArena)
	{
		mWalls.push_back(i);
	}

	for (size_t i = 0; i < pBlocks.size(); ++i)
	{
		auto sidesOccupied = checkForBlockFromSide(pBlocks, i);
		auto hasSide = [sidesOccupied](Side pSide) -> bool
			{
				return std::find(sidesOccupied.begin(), sidesOccupied.end(), pSide) != sidesOccupied.end();
			};

		SDL_FRect block = pBlocks[i];

		if (!hasSide(Side::RIGHT))
			mWalls.push_back(std::make_pair<Vector2f, Vector2f>({ block.x + block.w, block.y },
															    { block.x + block.w, block.y + block.h }));
		if (!hasSide(Side::LEFT))
			mWalls.push_back(std::make_pair<Vector2f, Vector2f>({ block.x, block.y },
																{ block.x, block.y + block.h }));
		if (!hasSide(Side::UP))
			mWalls.push_back(std::make_pair<Vector2f, Vector2f>({ block.x, block.y },
																{ block.x + block.w, block.y }));
		if (!hasSide(Side::DOWN))
			mWalls.push_back(std::make_pair<Vector2f, Vector2f>({ block.x, block.y + block.h },
																{ block.x + block.w, block.y + block.h }));
	}
}

auto Walls::createArena(const Vector2f& pWindow) -> void
{
	mArena[0] = std::make_pair<Vector2f, Vector2f>({ 10,10 }, { pWindow.x - 15, 10 });
	mArena[1] = std::make_pair<Vector2f, Vector2f>({ 10,10 }, { 10, pWindow.y - 15 });
	mArena[2] = std::make_pair<Vector2f, Vector2f>({ 10,pWindow.y - 15 }, { pWindow.x - 15, pWindow.y - 15 });
	mArena[3] = std::make_pair<Vector2f, Vector2f>({ pWindow.x - 15,10 }, { pWindow.x - 15, pWindow.y - 15 });
}

auto Walls::render(SDL_Renderer* pRenderer) -> void
{
	for (auto& i : mWalls)
	{
		SDL_RenderLine(pRenderer, i.first.x, i.first.y, i.second.x, i.second.y);
	}
	for (auto& i : mArena)
	{
		SDL_RenderLine(pRenderer, i.first.x, i.first.y, i.second.x, i.second.y);
	}
}

auto Walls::checkForBlockFromSide(std::vector<SDL_FRect>& pRangeSide, size_t pId) -> std::vector<Side>
{
	assert(pId < pRangeSide.size());
	std::vector<Side> tmpSides;
	tmpSides.reserve(4);

	auto check = [&](const SDL_FRect& pRect, Side pSide)
		{
			for (auto& i : pRangeSide)
			{
				if (SDL_HasRectIntersectionFloat(&i, &pRect))
					tmpSides.push_back(pSide);
			}
		};
	auto returnOrig = [&](SDL_FRect& pRect)
		{
			pRect = pRangeSide[pId];
			pRect.w = 1; 
		};

	SDL_FRect origRect = pRangeSide[pId];
	returnOrig(tmpRect);
	
	// checking right side
	tmpRect.x += origRect.w + 1;
	check(tmpRect, Side::RIGHT);
	returnOrig(tmpRect);
	
	// checking left side
	tmpRect.x -= 2;
	check(tmpRect, Side::LEFT);
	returnOrig(tmpRect);

	// checking up side
	tmpRect.w = origRect.w;
	tmpRect.h = 1;
	tmpRect.y -= 2;
	check(tmpRect, Side::UP);
	returnOrig(tmpRect);

	// checking down side
	tmpRect.w = origRect.w;
	tmpRect.h = 1;
	tmpRect.y += origRect.h + 1;
	check(tmpRect, Side::DOWN);

	return tmpSides;
}
