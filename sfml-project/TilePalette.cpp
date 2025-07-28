#include "stdafx.h"
#include "TilePalette.h"
#include "SceneEditor.h" 
#include "PrefabMgr.h"

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


void TilePalette::DrawImGui(SceneEditor& editor)
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

            if (ImGui::ImageButton(img, BTN_SZ))
                currentId = id;

            if (currentId == id)
            {
                ImGui::GetWindowDrawList()->AddRect(
                    ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                    IM_COL32(255, 255, 0, 255), 0.0F, 0, 2.0F);
            }

            if (++col < COLS) ImGui::SameLine();
            else              col = 0;
        }

        ImGui::EndChild();
        if (currentId >= 0)
        {
            const std::string& selName = editor.prefabNames[currentId];
            nlohmann::json& props = editor.prefabOverrides[selName];

            if (props.is_null())
                props = PrefabMgr::I().Get(selName)->defaultProps;

            ImGui::Separator();
            ImGui::Text("Props : %s", selName.c_str());

            sf::Vector2f& sc = editor.penScale[currentId];

            if (sc.x < 0.f) {                       // 처음 열었을 때만 기본값 복사
                const auto* a = PrefabMgr::I().Get(selName);
                sc = a ? a->scale : sf::Vector2f{ 1.f,1.f };
            }

            ImGui::InputFloat("scaleX", &sc.x, 0.01f, 0.1f, "%.3f");
            ImGui::InputFloat("scaleY", &sc.y, 0.01f, 0.1f, "%.3f");

            sc.x = Utils::Clamp(sc.x, 0.01f, 10.f);
            sc.y = Utils::Clamp(sc.y, 0.01f, 10.f);


            for (auto it = props.begin(); it != props.end(); ++it)
            {
                const char* key = it.key().c_str();

                if (it->is_boolean())
                {
                    bool val = it->get<bool>();
                    if (ImGui::Checkbox(key, &val))
                        *it = val;
                }
                else if (it->is_number_integer())
                {
                    int val = it->get<int>();
                    if (ImGui::InputInt(key, &val))
                        *it = val;
                }
                else if (it->is_number_float())
                {
                    float val = it->get<float>();
                    if (ImGui::InputFloat(key, &val))
                        *it = val;
                }
                else if (it->is_string())
                {
                    char buf[128]; 
                    std::snprintf(buf, sizeof(buf), "%s", it->get<std::string>().c_str());
                    //std::strncpy(buf, it->get<std::string>().c_str(), 128);
                    if (ImGui::InputText(key, buf, IM_ARRAYSIZE(buf)))
                        *it = std::string(buf);
                }
                else if (it->is_array())
                {
                    //ImGui::Text("%s : (array size %d)", key, (int)it->size());
                    if (it.key() == "path") {
                        if (it->size() == 2 &&
                            (*it)[0].is_array() && (*it)[0].size() == 2 &&
                            (*it)[1].is_array() && (*it)[1].size() == 2)
                        {
                            auto& arr = *it;          // alias

                            int p[4] = {
                                arr[0][0].get<int>(), arr[0][1].get<int>(),
                                arr[1][0].get<int>(), arr[1][1].get<int>()
                            };

                            if (ImGui::InputInt4("path (x1,y1,x2,y2)", p))
                            {
                                arr[0][0] = p[0];
                                arr[0][1] = p[1];
                                arr[1][0] = p[2];
                                arr[1][1] = p[3];
                            }
                        }
                    }
 
                }
                else
                {
                    ImGui::TextDisabled("%s : <unsupported>", key);
                }
            }
            editor.pendingProps = props;
        }
    }
    ImGui::End();
}
