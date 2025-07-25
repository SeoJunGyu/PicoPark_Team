#pragma once
#include "Gimmick.h"
class Button :
    public Gimmick
{
protected:
    int channel = 0; //같은 채널의 플랫폼을 이동시키기 위해 사용
    bool pressed = false;

    bool latch;

    static std::vector<int> momentaryCount; //채널별 누르고 있는 순간 수
    static std::vector<bool> latchState; //채널별 래치 on/off

public:
    Button(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;

    bool GetPressed() const { return pressed; }

    static void EnsureSize(int channel);
    static bool IsActive(int channel);
};

