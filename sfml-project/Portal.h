#pragma once
#include "Gimmick.h"
class Portal :
    public Gimmick
{
protected:
    int targetId = 0;
    Portal* target = nullptr;

public:
    Portal(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

