#include "Blocks.h"

void Blocks::init(int32_t pNumberCapacity)
{
    mBlocks.reserve(pNumberCapacity);
}

void Blocks::setDimension(size_t pIndex, float pW, float pH)
{
	if (pIndex < mBlocks.size())
	{
		mBlocks[pIndex].w = pW;
		mBlocks[pIndex].h = pH;
	}
}

void Blocks::setDimension(float pW, float pH)
{
	for (auto& block : mBlocks)
	{
		block.w = pW;
		block.h = pH;
	}
}

void Blocks::deleteSpecial(size_t pIndex) noexcept
{
	if (pIndex < mBlocks.size())
	{
		mBlocks.erase(mBlocks.begin() + pIndex);
	}
}

void Blocks::deleteAll() noexcept
{
	mBlocks.clear();
}

void Blocks::setAffectOnAll(bool pAffectOnAll)
{
    mAffectOnAll = pAffectOnAll;
}

void Blocks::control(const SDL_Event& pEvent)
{
    switch (pEvent.type)
    {
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (pEvent.button.button == SDL_BUTTON_RIGHT)
        {
            mBlocks.push_back({ mMousePos.x, mMousePos.y, 100,100 });
            mShouldMove = true;
        }
        if (pEvent.button.button == SDL_BUTTON_LEFT)
        {
            mShouldMove = false;
        }
        break;
    case SDL_EVENT_MOUSE_WHEEL:
        if (mBlocks.empty()) break;

        auto multiplier = pEvent.wheel.y > 0 ? 5 : -5;

        if (mAffectOnAll)
        {
            for (auto& i : mBlocks)
            {
                i.w += multiplier;
                i.h += multiplier;
            }
        }
        else
        {
            mBlocks[mCurrentBlock].w += multiplier;
            mBlocks[mCurrentBlock].h += multiplier;
        }

        break;
    }
}

void Blocks::setCurrentBlock(size_t pCurrentBlock) noexcept
{
    mCurrentBlock = pCurrentBlock;
}

size_t Blocks::getCurrentBlock() noexcept
{
    return mCurrentBlock;
}

const std::vector<SDL_FRect> Blocks::getStorage() noexcept
{
    return mBlocks;
}

size_t Blocks::getSizeStrg() noexcept
{
    return mBlocks.size();
}

void Blocks::update(const Vector2f& pMousePos)
{
    if (!mBlocks.empty() &&
         mShouldMove)
    {
        mBlocks[mBlocks.size() - 1].x = pMousePos.x;
        mBlocks[mBlocks.size() - 1].y = pMousePos.y;
    }
}

void Blocks::render(SDL_Renderer* pRenderer, bool pFilled)
{
    if (pFilled)
    {
        for (auto& i : mBlocks)
        {
            SDL_RenderRect(pRenderer, &i);
        }
    }
    else
    {
        for (auto& i : mBlocks)
        {
            SDL_RenderFillRect(pRenderer, &i);
        }
    }
}

