#include "stdafx.h"
#include "PrefabMgr.h"



PrefabMgr& PrefabMgr::I()
{
    static PrefabMgr instance;
    return instance;
}

void PrefabMgr::LoadAll(const fs::path& dir)
{
    for (auto& f : fs::directory_iterator(dir))
    {
        nlohmann::json j; std::ifstream(f.path()) >> j;
        PrefabAsset a;
        a.type = StrToType(j["type"]);
        a.sprite = j.value("sprite", "");
        a.scale = j.value("scale", sf::Vector2f( 1.f, 1.f ));
        a.defaultProps = j.value("properties", nlohmann::json::object());
        table[j["name"]] = std::move(a);
    }
}

const PrefabAsset* PrefabMgr::Get(const std::string& name) const
{
    auto it = table.find(name);
    return (it != table.end()) ? &it->second : nullptr;   // 없으면 nullptr
}

GameObject* PrefabMgr::Instantiate(const std::string& name,
    sf::Vector2f pos,
    const nlohmann::json& ov)
{
    const auto* asset = Get(name);
    if (!asset) { std::cerr << "[Prefab] not found: " << name << '\n'; return nullptr; }

    nlohmann::json j;
    j["id"] = -1;                     // 런타임용
    j["type"] = TypeToStr(asset->type);
    j["x"] = pos.x;  j["y"] = pos.y;
    j["scale"] = { asset->scale.x, asset->scale.y };
    j["properties"] = asset->defaultProps;
    for (auto it = ov.begin(); it != ov.end(); ++it)
    {
        const std::string& key = it.key();
        const nlohmann::json& val = it.value();
        j["properties"][key] = val;
    }

    return Gimmick::CreateFromJson(j);    // 이미 있는 공장 재활용
}
