#include "Setup.h"

Setup::Setup(int32_t pWidthWin, int32_t pHeightWin)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Couldnt initilize SDL!\n";
        exit(1);
    }
    mWindow = SDL_CreateWindow("2D light", pWidthWin, pHeightWin, SDL_WINDOW_RESIZABLE);
    mRenderer = SDL_CreateRenderer(mWindow, NULL);

    mWidthWin = pWidthWin;
    mHeightWin = pHeightWin;

    mUI.init(mWindow, mRenderer);
    mBlock.init(30);
    mWalls.createArena({ float(pWidthWin),float(pHeightWin) });
    mCircle.init(20, 150, { float(pWidthWin) / 2, float(pHeightWin) / 2 });
    mFactoryRays.pushRay({ mCircle.getPos().x, mCircle.getPos().y }, { .0f,.0f });
    mFactoryRays.pushRays({ mCircle.getPos().x, mCircle.getPos().y }, { .0f,.0f }, 200, 40.0f);

    mRenderTex = SDL_CreateTexture(mRenderer, 
                                  SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
                                  pWidthWin, pHeightWin);
    if (!mRenderTex)
        std::cout << "Couldnt create renderTex in setup()! Erorr: " << SDL_GetError() << '\n';
    mRenderTexLight = SDL_CreateTexture(mRenderer, 
                                       SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
                                       pWidthWin, pHeightWin);
    if (!mRenderTexLight)
        std::cout << "Couldnt create renderTexLight in setup()! Erorr: " << SDL_GetError() << '\n';
}

Setup::~Setup()
{
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}

void Setup::setFps(float pDelay) noexcept
{
    mDelay = pDelay;   
}

constexpr float Setup::getFps() const noexcept
{
    return mDelay;
}

const Vector2f Setup::getMousePos() noexcept
{
    return mMousePos;
}

constexpr bool Setup::programIsActive() noexcept
{
    return mIsActive;
}

void Setup::setProgActive(bool pIsActive) noexcept
{
    mIsActive = pIsActive;
}

bool Setup::pointInCircle()
{
    const float xPart = (mMousePos.x - mCircle.getPos().x);
    const float yPart = (mMousePos.y - mCircle.getPos().y);
    const float distanceFromPointToCircle = (xPart * xPart) + (yPart * yPart);
    const float squaredRadius = (mCircle.getBigRadius() * mCircle.getBigRadius());

    return distanceFromPointToCircle < squaredRadius;
}

void Setup::run()
{
    while (mIsActive)
    {
        SDL_Event event;
        SDL_GetMouseState(&mMousePos.x, &mMousePos.y);
        int32_t numKeys = 0;
        SDL_GetKeyboardState(&numKeys);
        mCircle.getStorageKeycodes().reserve(numKeys);

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
            {
                mIsActive = false;
                break;
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
                mCircle.getStorageKeycodes()[event.key.key] = true;
            if (event.type == SDL_EVENT_KEY_UP)
                mCircle.getStorageKeycodes()[event.key.key] = false;
            if (event.key.key == SDLK_ESCAPE)
            {
                mIsActive = false;
                break;
            }

            mBlock.control(event);
        }
        mCircle.control(5, mWidthWin, mHeightWin);
        mWalls.createWallsFromBlocks(mBlock.getStorage());

        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mRenderer);
        SDL_SetRenderDrawColor(mRenderer, (uint8_t)(mBlocksColor[0] * 255),
                                          (uint8_t)(mBlocksColor[1] * 255),
                                          (uint8_t)(mBlocksColor[2] * 255),
                                          255);

        const float firstFrame = (float)SDL_GetTicks();

        mUI.control(event, mIsActive, mBlocksColor, mCircleColor, mBlock, mCircle, mFactoryRays, mFilledBlocks);

        mBlock.update(mMousePos);
        mBlock.render(mRenderer, mFilledBlocks);

        mCircle.render(mRenderer);
        mWalls.render(mRenderer);
        mUI.render(mRenderer);
        
        if (pointInCircle())
        {
            glm::vec2 localVector = { mMousePos.x - mCircle.getPos().x, mMousePos.y - mCircle.getPos().y };
            localVector = glm::normalize(localVector) * float(mCircle.getBigRadius());
            const glm::vec2 globalVector = { localVector.x + mCircle.getPos().x, localVector.y + mCircle.getPos().y };

            mFactoryRays.update(mCircle.getPos(), { globalVector.x, globalVector.y }, true);
        }
        else
            mFactoryRays.update(mCircle.getPos(), { mMousePos.x, mMousePos.y }, true);
        mFactoryRays.render(mRenderer, mWalls);

        //renderShadows();

        SDL_RenderPresent(mRenderer);

        const float deltaTime = (float)SDL_GetTicks() - firstFrame;
        if (deltaTime < mDelay) 
            SDL_Delay((Uint32)(mDelay - deltaTime)); 

        adjustDeltaMoving();
    }
}

void Setup::adjustDeltaMoving() const
{
    const float currentTime = (float)SDL_GetTicks();
    float deltaTime2 = currentTime - mLastFrameRate;
    mLastFrameRate = currentTime;
    
    mCircle.setMultiplierSpeed(deltaTime2 * mUI.getTimeDoubler() * 0.01f);
}


//TODO: in the future maybe make instead of lines a triangle 
void Setup::renderShadows()
{
    SDL_SetRenderTarget(mRenderer, mRenderTexLight);
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
    SDL_RenderClear(mRenderer);


    auto sortedRays = mFactoryRays.getRayStorage();
    if (sortedRays.empty())
        return;

    auto sortCond = [this](const Ray& pFirst, const Ray& pSecond)
        {
            float angleA = atan2(pFirst.getEnd().x - mCircle.getPos().x,
                                 pFirst.getEnd().y - mCircle.getPos().y);
            float angleB = atan2(pSecond.getEnd().x - mCircle.getPos().x, 
                                 pSecond.getEnd().y - mCircle.getPos().y);
            return angleA < angleB;
        };
    std::sort(sortedRays.begin(), sortedRays.end(), sortCond);

    std::vector<SDL_Vertex> shadowVerts;
    shadowVerts.reserve(mFactoryRays.getNumber());

    const SDL_Color shadowColor = { 255,255,255,180 };
    const float shadowLength = mFactoryRays.getLength();

    for (size_t i = 0; i < sortedRays.size(); ++i)
    {
        size_t next = (i + 1) % sortedRays.size();
        SDL_FPoint lightPos = { mCircle.getPos().x, mCircle.getPos().y };
        SDL_FPoint dir1 =
        {
            sortedRays[i].getEnd().x - lightPos.x,
            sortedRays[i].getEnd().y - lightPos.y
        };
        SDL_FPoint dir2 =
        {
            sortedRays[next].getEnd().x - lightPos.x,
            sortedRays[next].getEnd().y - lightPos.y
        };

        float length1 = sqrt((dir1.x * dir1.x) + (dir1.y * dir1.y));
        float length2 = sqrt((dir2.x * dir2.x) + (dir2.x * dir2.y));

        SDL_FPoint point1 =
        {
            sortedRays[i].getEnd().x + (dir1.x / length1) * shadowLength,
            sortedRays[i].getEnd().y + (dir1.y / length1) * shadowLength
        };
        SDL_FPoint point2 =
        {
            sortedRays[next].getEnd().x + (dir2.x / length2) * shadowLength,
            sortedRays[next].getEnd().y + (dir2.y / length2) * shadowLength
        };

        SDL_Vertex vertecies[3] =
        {
            {{lightPos.x, lightPos.y},{shadowColor.r, shadowColor.g,
                                       shadowColor.b, shadowColor.a}, {0,0}},
            {{point1.x, point1.y},{shadowColor.r, shadowColor.g,
                                   shadowColor.b, shadowColor.a},{0,0}},
            {{point2.x, point2.y},{shadowColor.r, shadowColor.g,
                                   shadowColor.b, shadowColor.a},{0,0}}
        };
        shadowVerts.emplace_back(vertecies[0]);
        shadowVerts.emplace_back(vertecies[1]);
        shadowVerts.emplace_back(vertecies[2]);
    }
    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    SDL_RenderGeometry(mRenderer, nullptr, shadowVerts.data(), shadowVerts.size(), nullptr, 0);

    SDL_SetRenderTarget(mRenderer, nullptr);
    SDL_RenderTexture(mRenderer, mRenderTexLight, nullptr, nullptr);
}

