#include "stdafx.h"
#include "TilePalette.h"

void TilePalette::Load(const std::vector<std::string>& files, int ts)
{
    tileSet.tileSize = ts;
    tileSet.textures.clear();

    for (const std::string& f : files)
    {
        sf::Texture tex;
        if (!tex.loadFromFile(f))
        {
            std::cerr << "[TilePalette]  failed to load: " << f << '\n';
            // 채우지 않고 continue 하면 ID 정렬이 꼬인다. 더미 텍스처라도 push.
            sf::Image img;  img.create(ts, ts, sf::Color::Magenta);
            tex.loadFromImage(img);
        }
        tileSet.textures.push_back(std::move(tex));
    }
    currentId = -1;            
}

void TilePalette::DrawImGui()
{
    if (tileSet.textures.empty()) return;

    constexpr int COLS = 5;          // 한 줄 5칸
    const ImVec2 BTN_SZ(48, 48);     // 썸네일 크기

    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver); 
    ImGui::SetNextWindowPos(ImVec2(1300, 100), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Entities")) {
        ImGui::BeginChild("TilePaletteArea", ImVec2(0, 200), true,
            ImGuiWindowFlags_HorizontalScrollbar);

        int col = 0;
        for (int id = 0; id < (int)tileSet.textures.size(); ++id)
        {
            ImTextureID img = (ImTextureID)tileSet.textures[id].getNativeHandle();

            // 버튼: 클릭 시 currentId 변경
            if (ImGui::ImageButton(img, BTN_SZ))
                currentId = id;

            // 선택 테두리
            if (currentId == id)
            {
                ImGui::GetWindowDrawList()->AddRect(
                    ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                    IM_COL32(255, 255, 0, 255), 0.0F, 0, 2.0F);
            }

            // 줄 바꿈 처리
            if (++col < COLS) ImGui::SameLine();
            else              col = 0;
        }

        ImGui::EndChild();
    }
    ImGui::End();
}
