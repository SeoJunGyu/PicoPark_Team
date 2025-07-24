#pragma once
#include "Gimmick.h"
class Key :
    public Gimmick
{
protected:

public:
    Key(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

