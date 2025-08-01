#pragma once
#include "stdafx.h"
#include "Gimmick.h"

// 문자열 → enum
inline GimmickType StrToType(const std::string& s)
{
    if (s == "Key")         return GimmickType::Key;
    if (s == "Door")        return GimmickType::Door;
    if (s == "Button")        return GimmickType::Button;
    if (s == "PlayerSpawn") return GimmickType::PlayerSpawn;
    if (s == "MovingPlatform") return GimmickType::MovingPlatform;
    if (s == "BouncePad") return GimmickType::BouncePad;
    if (s == "FallingPlatform") return GimmickType::FallingPlatform;
    if (s == "RoundPlatform") return GimmickType::RoundPlatform;
    if (s == "PushBlock") return GimmickType::PushBlock;
    if (s == "Blinker") return GimmickType::Blinker;
    if (s == "Ghost") return GimmickType::Ghost;
    if (s == "Portal") return GimmickType::Portal;
    if (s == "Rope") return GimmickType::Rope;
    return GimmickType::Count;                     // 예외 대응
}

// enum → 문자열
inline const char* TypeToStr(GimmickType t)
{
    switch (t) {
    case GimmickType::Key:         return "Key";
    case GimmickType::Door:        return "Door";
    case GimmickType::Button:        return "Button";
    case GimmickType::PlayerSpawn: return "PlayerSpawn";
    case GimmickType::MovingPlatform: return "MovingPlatform";
    case GimmickType::BouncePad: return "BouncePad";
    case GimmickType::FallingPlatform: return "FallingPlatform";
    case GimmickType::RoundPlatform: return "RoundPlatform";
    case GimmickType::PushBlock: return "PushBlock";
    case GimmickType::Blinker: return "Blinker";
    case GimmickType::Ghost: return "Ghost";
    case GimmickType::Portal: return "Portal";
    case GimmickType::Rope: return "Rope";
    default:                       return "Unknown";
    }
}