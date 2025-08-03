#pragma once
#include "Program.h"
#include "Blocks.h"

enum class Side
{
	RIGHT = 0,
	LEFT = 1,
	UP = 2,
	DOWN = 3
};

class Walls
{
public:
	Walls();
	Walls(const std::pair<Vector2f, Vector2f> &pPosWall);
	Walls(std::span<std::pair<Vector2f,Vector2f>> pPosWalls);

	auto getWallsStrg() const noexcept -> std::vector<std::pair<Vector2f, Vector2f>>;
	auto createWallAroundBlock(std::vector<SDL_FRect>& pBlocks, size_t pId) -> void;
	auto createWallsFromBlocks(std::vector<SDL_FRect>& pBlocks) -> void;
	auto createArena(const Vector2f& pWindow) -> void;

	auto render(SDL_Renderer* pRenderer) -> void;

private:
	auto checkForBlockFromSide(std::vector<SDL_FRect>& pRangeSide, size_t pId) -> std::vector<Side>;

private:
	std::vector<std::pair<Vector2f, Vector2f>> mWalls;
	std::array<std::pair<Vector2f, Vector2f>, 4> mArena;

	SDL_FRect tmpRect{ 0.0f,0.0f,0.0f,0.0f };
};

