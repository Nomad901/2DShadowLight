#pragma once
#include "Program.h"

class Blocks
{
public:
	Blocks() = default;

	void init(int32_t pNumberCapacity);

	void setDimension(size_t pIndex, float pW, float pH);
	void setDimension(float pW, float pH);

	void deleteSpecial(size_t pIndex) noexcept;
	void deleteAll() noexcept;
	void setAffectOnAll(bool pAffectOnAll);
	void control(const SDL_Event& pEvent);
	void setCurrentBlock(size_t pCurrentBlock) noexcept;
	size_t getCurrentBlock() const noexcept;
	std::vector<SDL_FRect>& getStorage();

	size_t getSizeStrg() noexcept;

	void update(const Vector2f& pMousePos);
	void render(SDL_Renderer* pRenderer, bool pFilled);

private:
	bool mShouldMove{};
	bool mAffectOnAll{true};

	size_t mCurrentBlock{};

	std::vector<SDL_FRect> mBlocks;
	Vector2f mMousePos{};
};

