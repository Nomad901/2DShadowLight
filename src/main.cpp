#include "Setup.h"

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


void drawArena()
{
    SDL_RenderRect(Resources::sRenderer, &Resources::sArena);
}

void drawLine()
{
    Resources::Vector2f direction = { Resources::sVector2f.x - Resources::sCirclePos.x,
                                      Resources::sVector2f.y - Resources::sCirclePos.y };
    float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
    if (length > 0.0f)
    {
        direction.x /= length;
        direction.y /= length;
    }

    Resources::Vector2f endPoint = {
        Resources::sCirclePos.x + direction.x * Resources::sRadiusOfCircle,
        Resources::sCirclePos.y + direction.y * Resources::sRadiusOfCircle
    };

    SDL_RenderLine(Resources::sRenderer, Resources::sCirclePos.x, Resources::sCirclePos.y, endPoint.x, endPoint.y);
}

//void manageControlImGui()
//{
//    //
//    // Config for the blocks
//    //
//    static bool firstTime = true;
//    static bool anotherWindow = false;
//    if (firstTime)
//    {
//        ImGui::SetNextWindowPos({ 100,100 });
//        ImGui::SetNextWindowSize({ 520,180 });
//    }
//    ImGui::Begin("Control blocks", &Resources::isActive);
//
//    ImGui::Text("Small instruction: Right click - add new block");
//
//    ImGui::Spacing();
//    ImGui::Separator();
//
//    ImGui::Checkbox("Choose an exact block", &anotherWindow);
//
//    ImGui::Spacing();
//
//    if (ImGui::Button("Delete All blocks"))
//        Resources::sBlocks.clear();
//    if (ImGui::Button("Delete the exact block (choose this in the second window)"))
//    {
//        auto& tmpVctr = Resources::sBlocks;
//        if (!tmpVctr.empty())
//        {
//            tmpVctr.erase(tmpVctr.begin() + Resources::sCurrentBlock);
//            Resources::sCurrentBlock = tmpVctr.size() - 1;
//        }
//    }
//
//    ImGui::Spacing();
//    ImGui::Separator();
//
//    ImGui::ColorEdit3("Changing color of blocks", Resources::sBlocksColor);
//
//    ImGui::Spacing();
//    ImGui::Spacing();
//
//    ImGui::End();
//
//    //
//    // Choosing an exact block
//    //
//    if (anotherWindow)
//    {
//        static bool firstTime2 = true;
//        if (firstTime2)
//        {
//            ImGui::SetNextWindowPos({ 200,200 });
//            ImGui::SetNextWindowSize({ 200,400 });
//            firstTime2 = false;
//        }
//        ImGui::Begin("Choosing Block", &anotherWindow);
//
//        ImGui::Checkbox("Affect on all", &Resources::affectOnAll);
//
//        ImGui::Spacing();
//        ImGui::Separator();
//
//        for (size_t i = 0; i < Resources::sBlocks.size(); ++i)
//        {
//            if (ImGui::Button(std::to_string(i).c_str()))
//                Resources::sCurrentBlock = i;
//        }
//
//        ImGui::End();
//    }
//
//    //
//    // Circle config
//    //
//    if (firstTime)
//    {
//        ImGui::SetNextWindowPos({ 400,400 });
//        ImGui::SetNextWindowSize({ 300,300 });
//        firstTime = false;
//    }
//    ImGui::Begin("Circle config", &Resources::isActive);
//
//    ImGui::Spacing();
//
//    ImGui::SliderInt("Radius", &Resources::sRadiusOfCircle, 10, 100);
//
//
//    ImGui::Spacing();
//
//    ImGui::Checkbox("Filled circle", &Resources::filledCircle);
//
//    ImGui::Spacing();
//    ImGui::Separator();
//
//    ImGui::ColorEdit3("Color", Resources::sCircleColor);
//
//    ImGui::Spacing();
//    ImGui::Separator();
//
//    //ImGui::SliderFloat("Speed", &Resources::sSpeedCircle, 0.1f, 100.0f);
//    ImGui::SliderFloat("Speed", &Resources::sTimeDoubler, 0.1f, 100.0f);
//
//    ImGui::Spacing();
//    ImGui::Separator();
//
//    ImGui::End();
//}

//void controlBlocks(const SDL_Event& pEvents)
//{
//    switch (pEvents.type)
//    {
//    case SDL_EVENT_MOUSE_BUTTON_UP:
//        if (pEvents.button.button == SDL_BUTTON_RIGHT)
//        {
//            Resources::sBlocks.push_back({ Resources::sVector2f.x, Resources::sVector2f.y, 100,100 });
//            Resources::shouldMove = true;
//        }
//        if (pEvents.button.button == SDL_BUTTON_LEFT)
//        {
//            Resources::shouldMove = false;
//        }
//        break;
//    case SDL_EVENT_MOUSE_WHEEL:
//        if (Resources::sBlocks.empty()) break;
//
//        auto multiplier = pEvents.wheel.y > 0 ? 5 : -5;
//
//        if (Resources::affectOnAll)
//        {
//            for (auto& i : Resources::sBlocks)
//            {
//                i.w += multiplier;
//                i.h += multiplier;
//            }
//        }
//        else
//        {
//            Resources::sBlocks[Resources::sCurrentBlock].w += multiplier;
//            Resources::sBlocks[Resources::sCurrentBlock].h += multiplier;
//        }
//
//        break;
//    }
//}
//
//void controlCircle()
//{
//    float speed = Resources::sSpeedCircle * Resources::sMultiplierSpeed;
//    // and here we are trying to get access to our key which we pressed
//    // in addition it happens each frame
//    if (Resources::sKeys[SDLK_W])
//        Resources::sCirclePos.y -= speed;
//    if (Resources::sKeys[SDLK_S])
//        Resources::sCirclePos.y += speed;
//    if (Resources::sKeys[SDLK_D])
//        Resources::sCirclePos.x += speed;
//    if (Resources::sKeys[SDLK_A])
//        Resources::sCirclePos.x -= speed;
//
//    Resources::sCirclePos.x = std::clamp(Resources::sCirclePos.x, Resources::sArena.x, Resources::sArena.w);
//    Resources::sCirclePos.y = std::clamp(Resources::sCirclePos.y, Resources::sArena.y, Resources::sArena.h);
//}

int main(int argc, char* argv[])
{
    Setup setup(1280,720);
    setup.run();  

    return 0;
}
