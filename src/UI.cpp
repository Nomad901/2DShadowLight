#include "UI.h"

void UI::init(SDL_Window* pWindow, SDL_Renderer* pRenderer)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = NULL;

	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForSDLRenderer(pWindow, pRenderer);
	ImGui_ImplSDLRenderer3_Init(pRenderer);
}

const float UI::getTimeDoubler() const noexcept
{
    return mTimeDoubler;
}

void UI::control(const SDL_Event& pEvent, bool pActiveProg,
                 float pColors[3], float pCircleColor[3],
                 Blocks& pBlocks, Circle& pCircle, FactoryRays& pFactory,
                 bool& pFilledBlocks)
{
    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui::NewFrame();

    //
    // Block's config
    //
    static bool firstTime = true;
    static bool anotherWindow = false;
    if (firstTime)
    {
        ImGui::SetNextWindowPos({ 100,100 });
        ImGui::SetNextWindowSize({ 520,250 });
    }
    ImGui::Begin("Control blocks", &pActiveProg);

    ImGui::Text("Small instruction: Right click - add new block");

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Checkbox("Choose an exact block", &anotherWindow);

    ImGui::Spacing();

    if (ImGui::Button("Delete All blocks"))
        pBlocks.deleteAll();
    if (ImGui::Button("Delete the exact block (choose this in the second window)"))
        pBlocks.deleteSpecial(pBlocks.getCurrentBlock());
    
    ImGui::Spacing();
    ImGui::Separator();

    ImGui::ColorEdit3("Changing color of blocks", pColors);
    ImGui::Checkbox("Fill the blocks", &pFilledBlocks);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();

    static int32_t pNumberRays = 100;
    if (ImGui::SliderInt("Rays", &pNumberRays, 1, 500))
        pFactory.setNumberRays(pNumberRays);
    static float pLength = 50.0f;
    if (ImGui::SliderFloat("Range", &pLength, 10.0f, 500.0f))
        pFactory.setLength(pLength);

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::End();

    //
    // Choosing an exact block
    //
    if (anotherWindow)
    {
        static bool firstTime2 = true;
        static bool affectOnAll = false;
        if (firstTime2)
        {
            ImGui::SetNextWindowPos({ 200,200 });
            ImGui::SetNextWindowSize({ 200,400 });
            firstTime2 = false;
        }
        ImGui::Begin("Choosing Block", &anotherWindow);

        if(ImGui::Checkbox("Affect on all", &affectOnAll))
            pBlocks.setAffectOnAll(affectOnAll);

        ImGui::Spacing();
        ImGui::Separator();

        for (size_t i = 0; i < pBlocks.getSizeStrg(); ++i)
        {
            if (ImGui::Button(std::to_string(i).c_str()))
                pBlocks.setCurrentBlock(i);
        }

        ImGui::End();
    }

    //
    // Circle config
    //
    static int32_t tmpRadius = 30;
    static bool tmpFilledCircle = false;
    static float tmpMultiplierSpeed = 0;
    if (firstTime)
    {
        ImGui::SetNextWindowPos({ 400,400 });
        ImGui::SetNextWindowSize({ 300,300 });
        firstTime = false;
    }
    ImGui::Begin("Circle config", &pActiveProg);

    ImGui::Spacing();

    if (ImGui::SliderInt("Radius", &tmpRadius, 10, 100))
        pCircle.setRadius(tmpRadius, tmpRadius * 5);

    ImGui::Spacing();

    if(ImGui::Checkbox("Empty circle", &tmpFilledCircle))
        pCircle.setCircleFilled(tmpFilledCircle);

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::ColorEdit3("Color", pCircleColor))
        pCircle.setCircleColor(pCircleColor);

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::SliderFloat("Speed", &mTimeDoubler, 0.1f, 100.0f);

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::End();
}

void UI::render(SDL_Renderer* pRenderer)
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), pRenderer);
}
