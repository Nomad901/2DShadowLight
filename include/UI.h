#pragma once
#include <Program.h>
#include <Blocks.h>
#include <Circle.h>

class UI
{
public:
	void init(SDL_Window* pWindow, SDL_Renderer* pRenderer);

	const float getTimeDoubler() const noexcept;

	void control(const SDL_Event& pEvent, bool pActiveProg,
				 float pColors[3], float pCircleColor[3], 
				 Blocks& pBlocks, Circle& pCircle);
	void render(SDL_Renderer* pRenderer);

private:
	float mTimeDoubler{};
};

