#pragma once
#include "Gimmick.h"
class Door :
    public Gimmick
{
protected:

public:
    Door(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

