#pragma once
#include <Program.h>

class Circle
{
public:
	void init(int32_t pRadius, const Vector2f& pPos);

	void setRadius(int32_t pRadius) noexcept;
	constexpr int32_t getRadius() noexcept;

	void setPos(Vector2f pPos) noexcept;
	constexpr Vector2f getPos() noexcept;
	
	void setCircleFilled(bool pFilledCircle) noexcept;
	void setCircleColor(float pCircleColor[3]) noexcept;
	void setMultiplierSpeed(float pMultiplierSpeed) noexcept;

	std::unordered_map<SDL_Keycode, bool>& getStorageKeycodes();

	void control(float pSpeed, float pArenaWidth, float pArenaHeight);
	void render(SDL_Renderer* pRenderer) const;
	
private:
	bool mFilledCircle{ false };

	int32_t mRadius{};
	float mSpeed{};
	float mMultiplierSpeed{};

	std::array<float, 3> mCircleColor = { 1.0f,1.0f,1.0f };

	Vector2f mPos{};
	std::unordered_map<SDL_Keycode, bool> mKeycodes;
};
