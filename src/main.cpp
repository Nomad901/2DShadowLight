#include <iostream>
#include <format>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <expected>
#include <span>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL3_gfxPrimitives.h>

namespace Resources
{
    bool isActive = true;
    bool shouldMove = false;
    bool affectOnAll = true;
    bool filledCircle = false;

    const uint32_t WIN_WIDTH{ 1280 }, WIN_HEIGHT{ 720 };

    uint32_t sCurrentBlock{};
    int32_t sRadiusOfCircle{ 30 };
    uint32_t sDelay{ 14 };

    float sSpeedCircle{ 15.0f };
    float sMultiplierSpeed{ 1.0f };
    float sLastFrameRate{ 1.0f };
    float sTimeDoubler{ 50.0f };

    SDL_Window* sWindow{};
    SDL_Renderer* sRenderer{};

    float sCircleColor[3] = { 1.0f, 1.0f, 1.0f };
    float sBlocksColor[3] = { 1.0f, 1.0f, 1.0f };

    std::vector<SDL_FRect> sBlocks;
    std::unordered_map<SDL_Keycode, bool> sKeys;

    struct Vector2f
    {
        float x{ WIN_WIDTH / 2 }, y{ WIN_HEIGHT / 2 };

        float distanceTo(Vector2f pOther)
        {
            float dx = x - pOther.x;
            float dy = y - pOther.y;
            return sqrt(dx * dx + dy * dy);
        }

    } sVector2f;
    Vector2f sCirclePos{};
    
}

class Wall
{
public:
    Wall(Resources::Vector2f pBeginningPos, Resources::Vector2f pEndPos)
    {
        mBeginningWall = pBeginningPos;
        mEndWall = pEndPos;
    }

    void setBeginningPos(Resources::Vector2f pBeginningPos)
    {
        mBeginningWall = pBeginningPos;
    }
    void setEndPos(Resources::Vector2f pEndPos)
    {
        mEndWall = pEndPos;
    }
    Resources::Vector2f getBeginningPos()
    {
        return mBeginningWall;
    }
    Resources::Vector2f getEndPos()
    {
        return mEndWall;
    }

    void render()
    {
        SDL_RenderLine(Resources::sRenderer, mBeginningWall.x, mBeginningWall.y, mEndWall.x, mEndWall.y);
    }

private:
    Resources::Vector2f mBeginningWall{};
    Resources::Vector2f mEndWall{};
};

class Ray
{
public:
    Ray(Resources::Vector2f pBeginningPos, Resources::Vector2f pEndPos, float pAngle)
    {
        mBeginningRay = pBeginningPos;

        const float length = 50.0f;
        mEndRay.x = pEndPos.x + length * cosf(pAngle);
        mEndRay.y = pEndPos.y + length * sinf(pAngle);
    }

    void setBeginningPos(Resources::Vector2f pBeginningPos)
    {
        mBeginningRay = pBeginningPos;
    }
    void setEndPos(Resources::Vector2f pEndPos)
    {
        mEndRay = pEndPos;
    }
    void setEndPos(Resources::Vector2f pEndPos, float pAngle)
    {
        const float length = 50.0f;
        mEndRay.x = pEndPos.x + length * cosf(pAngle);
        mEndRay.y = pEndPos.y + length * sinf(pAngle);
    }
    Resources::Vector2f getBeginningPos()
    {
        return mBeginningRay;
    }
    Resources::Vector2f getEndPos()
    {
        return mEndRay;
    }

    auto collide(Wall pWall) -> std::expected<Resources::Vector2f, bool>
    {
        const float x1 = pWall.getBeginningPos().x;
        const float y1 = pWall.getBeginningPos().y;
        const float x2 = pWall.getEndPos().x;
        const float y2 = pWall.getEndPos().y;

        const float x3 = mBeginningRay.x;
        const float y3 = mBeginningRay.y;
        const float x4 = mEndRay.x;
        const float y4 = mEndRay.y;

        float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (den == 0)
            return std::unexpected(false);

        mT = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
        mU = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
   
        if (mT > 0 && mT < 1 && mU > 0)
        {
            Resources::Vector2f pointVctr = { x1 + mT * (x2 - x1), 
                                              y1 + mT * (y2 - y1) };
            return pointVctr;
        }
        return std::unexpected(false);
    }

    void render()
    {
        SDL_RenderLine(Resources::sRenderer, mBeginningRay.x, mBeginningRay.y, mEndRay.x, mEndRay.y);
    }

private:
    Resources::Vector2f mBeginningRay{};
    Resources::Vector2f mEndRay{};

    float mT{}, mU{};
};

class Particle
{
public:
    Particle()
    {
        Resources::Vector2f pos = { 500, 400 };
        mStrgRays.reserve(40);
        for (int32_t i = 0; i < 40; i++)
        {
            Ray newRay = { Resources::sVector2f, {500,400}, float(float(i) * 3.14 / M_PI)};
            mStrgRays.emplace_back(std::move(newRay));
        }
    }

    void update()
    {
        for (int32_t i = 0; i < std::ssize(mStrgRays); ++i)
        {
            mStrgRays[i].setBeginningPos(Resources::sVector2f);
            mStrgRays[i].setEndPos({ Resources::sVector2f.x +10,Resources::sVector2f.y + 10 }, float(i));
            mStrgRays[i].render();
        }
    }
    void render(std::span<Wall> pWall)
    {
        for (auto& ray : mStrgRays)
        {
            Resources::Vector2f closest = { -100.0f,-100.0f };
            float record = std::numeric_limits<float>::infinity();
            for (auto& wall : pWall)
            {
                wall.render();
                ray.render();
                auto value = ray.collide(wall);
                if (value.has_value())
                {
                    float distance = ray.getBeginningPos().distanceTo(*value);
                    if (distance < record)
                    {
                        record = distance;
                        closest = *value;
                    }
                }
            }
            if (record < std::numeric_limits<float>::infinity())
            {
                SDL_RenderLine(Resources::sRenderer, ray.getBeginningPos().x, ray.getBeginningPos().y, closest.x, closest.y);
            }
            filledCircleRGBA(Resources::sRenderer, Resources::sVector2f.x, Resources::sVector2f.y, 10, 255, 255, 255, 255);
        }
    }

private:
    std::vector<Ray> mStrgRays;
};

int main(int argc, char* argv[])
{
    //
    //  SDL3 Part
    //
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Couldnt initilize SDL!\n";
        exit(1);
    }
    Resources::sWindow = SDL_CreateWindow("2D light", Resources::WIN_WIDTH, Resources::WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
    Resources::sRenderer = SDL_CreateRenderer(Resources::sWindow, NULL);
    std::vector<Wall> strgWalls
    {
        {{1000,200}, {1000,600}},
        {{500, 200}, {500,600}},
        {{432,213}, {54,34}},
        {{10, 10}, {1200, 10}},
        {{10,10}, {10, 1200 }},
        {{1200, 10}, {10, 1200}},
        {{10,1200}, {1200, 10}}
    };

    Particle particle;
    // ----------
    while (Resources::isActive)
    {
        SDL_Event event;
        SDL_GetMouseState(&Resources::sVector2f.x, &Resources::sVector2f.y);
        int32_t numKeys = 0;
        SDL_GetKeyboardState(&numKeys);
        Resources::sKeys.reserve(numKeys);
        static int32_t counter = 10;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                Resources::isActive = false;
                break;
            }
        }
        SDL_SetRenderDrawColor(Resources::sRenderer, 0, 0, 0, 255);
        SDL_RenderClear(Resources::sRenderer);
        SDL_SetRenderDrawColor(Resources::sRenderer, (uint8_t)(Resources::sBlocksColor[0] * 255),
                                                     (uint8_t)(Resources::sBlocksColor[1] * 255),
                                                     (uint8_t)(Resources::sBlocksColor[2] * 255),
                                                     1);
        float firstFrame = (float)SDL_GetTicks();
        
        
        //wall.render();
        particle.update();
        particle.render(strgWalls);
        SDL_RenderPresent(Resources::sRenderer);

        float deltaTime = (float)SDL_GetTicks() - firstFrame;
        if (deltaTime < Resources::sDelay) 
            SDL_Delay((Uint32)(Resources::sDelay - deltaTime)); 
    }

    SDL_DestroyWindow(Resources::sWindow);
    SDL_DestroyRenderer(Resources::sRenderer);
    SDL_Quit();
    return 0;
}
