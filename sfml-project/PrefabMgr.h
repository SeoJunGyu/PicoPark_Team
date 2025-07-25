#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace nlohmann {
    template<>
    struct adl_serializer<sf::Vector2f>   // from_json
    {
        static void from_json(const json& j, sf::Vector2f& v)
        {
            if (j.is_array()) { v.x = j[0]; v.y = j[1]; }
            else v.x = v.y = j.get<float>();
        }
        static void to_json(json& j, const sf::Vector2f& v)
        {
            j = json::array({ v.x, v.y });
        }
    };
}

struct PrefabAsset {
    GimmickType type;
    sf::Vector2f scale{ 1.f, 1.f };
    std::string sprite;
    nlohmann::json defaultProps;       // prop값
};

class PrefabMgr {
public:
    static PrefabMgr& I();

    void LoadAll(const fs::path& dir);
    const PrefabAsset* Get(const std::string& name) const;
    const std::unordered_map<std::string, PrefabAsset>& Table() const { return table; }

    /** @brief 프리팹 이름 + 오버라이드 → 실제 Gimmick* 반환 */
    GameObject* Instantiate(const std::string& name,
        sf::Vector2f pos,
        const nlohmann::json& overrideProps = {});
private:
    std::unordered_map<std::string, PrefabAsset> table;
};

