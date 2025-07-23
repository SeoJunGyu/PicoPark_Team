#pragma once
#include "stdafx.h"
#include "Gimmick.h"

// 문자열 → enum
inline GimmickType StrToType(const std::string& s)
{
    if (s == "Key")         return GimmickType::Key;
    if (s == "Door")        return GimmickType::Door;
    if (s == "PlayerSpawn") return GimmickType::PlayerSpawn;
    return GimmickType::Count;                     // 예외 대응
}

// enum → 문자열
inline const char* TypeToStr(GimmickType t)
{
    switch (t) {
    case GimmickType::Key:         return "Key";
    case GimmickType::Door:        return "Door";
    case GimmickType::PlayerSpawn: return "PlayerSpawn";
    default:                       return "Unknown";
    }
}