#pragma once
#include "Gimmick.h"
class Button :
    public Gimmick
{
protected:
    int channel = 0; //같은 채널의 플랫폼을 이동시키기 위해 사용
    bool pressed = false;

    bool latch;

public:
    Button(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;

    bool GetPressed() const { return pressed; }
};

