#pragma once
#include "Gimmick.h"
class BouncePad :
    public Gimmick
{
protected:
    Player* player = nullptr;

    float bounce;

public:
    BouncePad(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

