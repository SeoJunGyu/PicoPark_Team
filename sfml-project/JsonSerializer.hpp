#pragma once
#include "stdafx.h"
#include "GimmickTypeConv.hpp"

// Layer
 inline void to_json(nlohmann::json& j, const Layer& l) {
    j = { {"name",l.name},{"type", l.type == Layer::Kind::TILE ? "TILE" : "OBJECT"},
         {"visible",l.visible},{"parallax",l.parallax},{"data",l.data} };
}
 inline void from_json(const nlohmann::json& j, Layer& l) {
    l.name = j.at("name").get<std::string>();
    l.type = j.at("type").get<std::string>() == "TILE" ? Layer::Kind::TILE : Layer::Kind::OBJECT;
    l.visible = j.value("visible", true);
    l.parallax = j.value("parallax", 1.f);
    l.data = j.at("data").get<std::vector<int>>();
}
 
// Entity
 inline void to_json(nlohmann::json& j, const Entity& e) {
    j = { {"id",e.id},{"type",TypeToStr(e.type)},{"x",e.x},{"y",e.y} };
    if (e.w != 16 || e.h != 16) { j["w"] = e.w; j["h"] = e.h; }
    if (e.scale.x != 1.f || e.scale.y != 1.f)
        j["scale"] = { e.scale.x, e.scale.y };
    if (!e.properties.empty()) j["properties"] = e.properties;
}
 inline void from_json(const nlohmann::json& j, Entity& e) {
    e.id = j.at("id");
    e.type = StrToType(j.at("type"));
    e.x = j.at("x");  e.y = j.at("y");
    e.w = j.value("w", 16); e.h = j.value("h", 16);
    if (j.contains("scale"))
    {
        auto v = j["scale"];
        e.scale = { v.at(0).get<float>(), v.at(1).get<float>() };
    }
    else
        e.scale = { 1.f,1.f };
    if (j.contains("properties")) e.properties = j["properties"];
}

// Level
 inline void to_json(nlohmann::json& j, const Level& lv) {
    j = { {"version",1},{"tileSize",lv.tileSize},
         {"gridWidth",lv.gridWidth},{"gridHeight",lv.gridHeight},
         {"author",lv.author},{"description",lv.description},
         {"layers",lv.layers},{"entities",lv.entities} };
}
 inline void from_json(const nlohmann::json& j, Level& lv) {
    lv.tileSize = j.at("tileSize");
    lv.gridWidth = j.at("gridWidth");
    lv.gridHeight = j.at("gridHeight");
    lv.author = j.value("author", "");
    lv.description = j.value("description", "");
    lv.layers = j.at("layers").get<std::vector<Layer>>();
    lv.entities = j.at("entities").get<std::vector<Entity>>();

    if (!lv.layers.empty() && lv.layers[0].type == Layer::Kind::TILE)
        lv.tiles = lv.layers[0].data;
}
