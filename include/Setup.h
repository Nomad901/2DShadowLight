#pragma once
#include <UI.h>
#include <Walls.h>
#include <FactoryRays.h>
#include <glm.hpp>

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

	bool pointInCircle();

private:
	bool mIsActive{true};
	bool mFilledBlocks{ false };

	float mDelay{ 16 };
	mutable float mLastFrameRate{};

	float mBlocksColor[3]{ 0.3f, 0.2f, 0.5f };
	float mCircleColor[3]{ 1.0f, 1.0f, 1.0f };

	int32_t mWidthWin{ 1280 }, mHeightWin{ 720 };

	SDL_Renderer* mRenderer{};
	SDL_Window* mWindow{};
	
	FactoryRays mFactoryRays;
	Walls mWalls;
	Blocks mBlock;
	mutable Circle mCircle;
	UI mUI;

	SDL_Texture* renderTex;
	SDL_Texture* renderTexLight;

	Vector2f mMousePos;
};

