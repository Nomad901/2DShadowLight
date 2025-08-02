#pragma once
#include <Ray.h>
#include <UI.h>

enum class FuncType
{
	PREDRAW = 0,
	INPUT = 1,
	DRAW = 2
};

class Setup
{
public:
	Setup(int32_t pWidthWin, int32_t pHeightWin);
	~Setup();
	Setup(const Setup&) = delete;
	Setup operator=(const Setup&) = delete;

	void setFps(float pDelay) noexcept;
	constexpr float getFps() const noexcept;

	const Vector2f getMousePos() noexcept;
	constexpr bool programIsActive() noexcept;
	void setProgActive(bool pIsActive) noexcept;

	void run();
private:
	void adjustDeltaMoving() const;

private:
	bool mIsActive{true};
	float mDelay{ 16 };
	mutable float mLastFrameRate{};
	float mBlocksColor[3];
	float mCircleColor[3];

	int32_t mWidthWin{}, mHeightWin{};

	SDL_Renderer* mRenderer{};
	SDL_Window* mWindow{};
	
	Blocks mBlock;
	mutable Circle mCircle;
	Ray mRay;
	UI mUI;
	Vector2f mMousePos;
};

