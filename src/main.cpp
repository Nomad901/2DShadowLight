#include <iostream>
#include <format>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

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
    uint32_t sDelay{ 16 };

    float sSpeedCircle{ 15.0f };
    float sMultiplierSpeed{ 1.0f };
    float sLastFrameRate{ 1.0f };
    float sTimeDoubler{ 50.0f };

    SDL_Window* sWindow{};
    SDL_Renderer* sRenderer{};
    SDL_FRect sArena = { 10, 10, WIN_WIDTH - 20, WIN_HEIGHT - 20 };

    float sCircleColor[3] = { 1.0f, 1.0f, 1.0f };
    float sBlocksColor[3] = { 1.0f, 1.0f, 1.0f };

    std::vector<SDL_FRect> sBlocks;
    std::unordered_map<SDL_Keycode, bool> sKeys;

    struct Vector2f
    {
        float x{ WIN_WIDTH / 2 }, y{ WIN_HEIGHT / 2 };
    } sVector2f;
    Vector2f sCirclePos;
}

void drawCircle()
{
    if (!Resources::filledCircle)
    {
        circleRGBA(Resources::sRenderer, Resources::sCirclePos.x, Resources::sCirclePos.y,
            Resources::sRadiusOfCircle,
            (uint8_t)(Resources::sCircleColor[0] * 255),
            (uint8_t)(Resources::sCircleColor[1] * 255),
            (uint8_t)(Resources::sCircleColor[2] * 255),
            255);
    }
    else
    {
        filledCircleRGBA(Resources::sRenderer, Resources::sCirclePos.x, Resources::sCirclePos.y,
            Resources::sRadiusOfCircle,
            (uint8_t)(Resources::sCircleColor[0] * 255),
            (uint8_t)(Resources::sCircleColor[1] * 255),
            (uint8_t)(Resources::sCircleColor[2] * 255),
            255);
    }
}

void drawArena()
{
    SDL_RenderRect(Resources::sRenderer, &Resources::sArena);
}

void manageControlImGui()
{
    //
    // Config for the blocks
    //
    static bool firstTime = true;
    static bool anotherWindow = false;
    if (firstTime)
    {
        ImGui::SetNextWindowPos({ 100,100 });
        ImGui::SetNextWindowSize({ 520,180 });
    }
    ImGui::Begin("Control blocks", &Resources::isActive);

    ImGui::Text("Small instruction: Right click - add new block");

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Checkbox("Choose an exact block", &anotherWindow);

    ImGui::Spacing();

    if (ImGui::Button("Delete All blocks"))
        Resources::sBlocks.clear();
    if (ImGui::Button("Delete the exact block (choose this in the second window)"))
    {
        auto& tmpVctr = Resources::sBlocks;
        if (!tmpVctr.empty())
        {
            tmpVctr.erase(tmpVctr.begin() + Resources::sCurrentBlock);
            Resources::sCurrentBlock = tmpVctr.size() - 1;
        }
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::ColorEdit3("Changing color of blocks", Resources::sBlocksColor);

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::End();

    //
    // Choosing an exact block
    //
    if (anotherWindow)
    {
        static bool firstTime2 = true;
        if (firstTime2)
        {
            ImGui::SetNextWindowPos({ 200,200 });
            ImGui::SetNextWindowSize({ 200,400 });
            firstTime2 = false;
        }
        ImGui::Begin("Choosing Block", &anotherWindow);

        ImGui::Checkbox("Affect on all", &Resources::affectOnAll);

        ImGui::Spacing();
        ImGui::Separator();

        for (size_t i = 0; i < Resources::sBlocks.size(); ++i)
        {
            if (ImGui::Button(std::to_string(i).c_str()))
                Resources::sCurrentBlock = i;
        }

        ImGui::End();
    }

    //
    // Circle config
    //
    if (firstTime)
    {
        ImGui::SetNextWindowPos({ 400,400 });
        ImGui::SetNextWindowSize({ 300,300 });
        firstTime = false;
    }
    ImGui::Begin("Circle config", &Resources::isActive);

    ImGui::Spacing();

    ImGui::SliderInt("Radius", &Resources::sRadiusOfCircle, 10, 100);


    ImGui::Spacing();

    ImGui::Checkbox("Filled circle", &Resources::filledCircle);

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::ColorEdit3("Color", Resources::sCircleColor);

    ImGui::Spacing();
    ImGui::Separator();

    //ImGui::SliderFloat("Speed", &Resources::sSpeedCircle, 0.1f, 100.0f);
    ImGui::SliderFloat("Speed", &Resources::sTimeDoubler, 0.1f, 100.0f);

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::End();
}

void controlBlocks(const SDL_Event& pEvents)
{
    switch (pEvents.type)
    {
    case SDL_EVENT_QUIT:
        Resources::isActive = false;
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (pEvents.button.button == SDL_BUTTON_RIGHT)
        {
            Resources::sBlocks.push_back({ Resources::sVector2f.x, Resources::sVector2f.y, 100,100 });
            Resources::shouldMove = true;
        }
        if (pEvents.button.button == SDL_BUTTON_LEFT)
        {
            Resources::shouldMove = false;
        }
        break;
    case SDL_EVENT_MOUSE_WHEEL:
        if (Resources::sBlocks.empty()) break;

        auto multiplier = pEvents.wheel.y > 0 ? 5 : -5;

        if (Resources::affectOnAll)
        {
            for (auto& i : Resources::sBlocks)
            {
                i.w += multiplier;
                i.h += multiplier;
            }
        }
        else
        {
            Resources::sBlocks[Resources::sCurrentBlock].w += multiplier;
            Resources::sBlocks[Resources::sCurrentBlock].h += multiplier;
        }

        break;
    }
}

void controlCircle()
{
    float speed = Resources::sSpeedCircle * Resources::sMultiplierSpeed;
    // and here we are trying to get access to our key which we pressed
    // in addition it happens each frame
    if (Resources::sKeys[SDLK_W])
        Resources::sCirclePos.y -= speed;
    if (Resources::sKeys[SDLK_S])
        Resources::sCirclePos.y += speed;
    if (Resources::sKeys[SDLK_D])
        Resources::sCirclePos.x += speed;
    if (Resources::sKeys[SDLK_A])
        Resources::sCirclePos.x -= speed;

    Resources::sCirclePos.x = std::clamp(Resources::sCirclePos.x, Resources::sArena.x, Resources::sArena.w);
    Resources::sCirclePos.y = std::clamp(Resources::sCirclePos.y, Resources::sArena.y, Resources::sArena.h);
}

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
    // ----------
    Resources::sBlocks.reserve(30);
    // 
    // ImGui part
    //
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& tmpIo = ImGui::GetIO();

    tmpIo.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    tmpIo.IniFilename = NULL;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(Resources::sWindow, Resources::sRenderer);
    ImGui_ImplSDLRenderer3_Init(Resources::sRenderer);

    // ----------
    while (Resources::isActive)
    {
        SDL_Event event;
        SDL_GetMouseState(&Resources::sVector2f.x, &Resources::sVector2f.y);
        int32_t numKeys = 0;
        SDL_GetKeyboardState(&numKeys);
        Resources::sKeys.reserve(numKeys);
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            controlBlocks(event);
            // moving without delay in the beginning: 
            // here we set true for a key, which we pressed
            // it doesnt happen each frame, thats the reason of delaying 
            // at the beginning of moving
            if (event.type == SDL_EVENT_KEY_DOWN)
                Resources::sKeys[event.key.key] = true;
            if (event.type == SDL_EVENT_KEY_UP)
                Resources::sKeys[event.key.key] = false;
        }
        controlCircle();
        SDL_SetRenderDrawColor(Resources::sRenderer, 0, 0, 0, 255);
        SDL_RenderClear(Resources::sRenderer);
        SDL_SetRenderDrawColor(Resources::sRenderer, (uint8_t)(Resources::sBlocksColor[0] * 255),
                                                     (uint8_t)(Resources::sBlocksColor[1] * 255),
                                                     (uint8_t)(Resources::sBlocksColor[2] * 255),
                                                     255);
        // getting time of the first frame
        float firstFrame = (float)SDL_GetTicks();


        if (!Resources::sBlocks.empty() &&
            Resources::shouldMove)
        {
            Resources::sBlocks[Resources::sBlocks.size() - 1].x = Resources::sVector2f.x;
            Resources::sBlocks[Resources::sBlocks.size() - 1].y = Resources::sVector2f.y;
        }
        for (auto& i : Resources::sBlocks)
        {
            SDL_RenderRect(Resources::sRenderer, &i);
        }

        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui::NewFrame();

        manageControlImGui();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), Resources::sRenderer);

        drawArena();
        drawCircle();
        SDL_RenderPresent(Resources::sRenderer);

        // setting the proper delay
        float deltaTime = (float)SDL_GetTicks() - firstFrame;
        // if deltaTime > 16 - then we will miss slowing our program down
        // if deltaTime < 16 - we will slow it down. Like 16 - deltatime
        // and we will get a desired number which can help slow our program down
        if (deltaTime < Resources::sDelay) // let it be 8 and 16
            SDL_Delay((Uint32)(Resources::sDelay - deltaTime)); // 16-8 = 8 
        // in order to make our fps properly, we need to slow down our program on 8 miliseconds 
        // with this equation we can do this 

        // multiplications for circle's speed
        float currentTime = (float)SDL_GetTicks(); // current time of the frame
        float deltaTime2 = currentTime - Resources::sLastFrameRate; // delta time from the last frame. usually it 16 fps which we set 
        Resources::sLastFrameRate = currentTime; // setting our frame as a last frame

        // cap delta time to prevent jumps
        const float MAX_DELTA_TIME = 50.0f;
        if (deltaTime2 > MAX_DELTA_TIME)
            deltaTime2 = MAX_DELTA_TIME;

        // convert to seconds and apply speed multiplier
        Resources::sMultiplierSpeed = deltaTime2 * Resources::sTimeDoubler * 0.001f;
    }

    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(Resources::sWindow);
    SDL_DestroyRenderer(Resources::sRenderer);
    SDL_Quit();
    return 0;
}
