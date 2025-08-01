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
    mUI.init(mWindow, mRenderer);
    mBlock.init(30);
    mCircle.init(20, { float(pWidthWin) / 2, float(pHeightWin) / 2 });
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
        }
        mCircle.control(5, mWidthWin, mHeightWin);

        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mRenderer);
        SDL_SetRenderDrawColor(mRenderer, (uint8_t)(mBlocksColor[0] * 255),
                                          (uint8_t)(mBlocksColor[1] * 255),
                                          (uint8_t)(mBlocksColor[2] * 255),
                                          255);
        const float firstFrame = (float)SDL_GetTicks();

        mUI.control(event, mIsActive, mBlocksColor, mCircleColor, mBlock, mCircle);

        mBlock.control(event);
        mBlock.update(mMousePos);
        mBlock.render(mRenderer, false);

      
        mCircle.render(mRenderer);
        mUI.render(mRenderer);
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
    const float deltaTime2 = currentTime - mDelay;
    mLastFrameRate = currentTime;

    mCircle.setMultiplierSpeed(deltaTime2 * mUI.getTimeDoubler() * 0.001f);
}
