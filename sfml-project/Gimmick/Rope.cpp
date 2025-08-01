#include "stdafx.h"
#include "Rope.h"

Rope::Rope(nlohmann::json j)
    : Gimmick(j.value("id", 0), StrToType("Rope"),
        { j.at("x"), j.at("y") }, { 1,1 }, 0.f,
        j.value("properties", nlohmann::json::object()))
{
}

void Rope::Reset()
{
    nodes = Variables::players;  
    segLen = properties.value("segLen", 36);
    springK = properties.value("spring", 20.f);
    Gimmick::Reset();
}

void Rope::Update(float dt)
{
    if (nodes.empty()) return;

    for (size_t i = 1; i < nodes.size(); ++i)
    {
        Player* prev = nodes[i - 1];
        Player* cur = nodes[i];

        sf::Vector2f delta = cur->GetPosition() - prev->GetPosition();
        float        dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (dist > segLen)
        {
            // (dist - segLen) 만큼 뒤쪽을 끌어당겨 준다
            sf::Vector2f dir = delta / dist;          // 정규화
            float overShoot = dist - segLen;

            // 단순 위치 이동(teleport) 보단 ‘가속도’로 힘을 주면 자연스럽다
            //cur->velocity -= dir * springK * overShoot * dt;
            prev->SetPosition(prev->GetPosition() + dir * overShoot * 0.5f);
            cur->SetPosition(cur->GetPosition() - dir * overShoot * 0.5f);

            // 원한다면 선두 쪽에 반대 힘 +dir 도 줄 수 있음 (뉴턴 3법칙)
        }
    }

    Gimmick::Update(dt);
}

//void Rope::Draw(sf::RenderWindow& w)
//{
//    if (nodes.size() < 2) return;
//
//    sf::VertexArray line(sf::LineStrip, nodes.size());
//    for (size_t i = 0; i < nodes.size(); ++i)
//    {
//        line[i].position = nodes[i]->GetPosition() + sf::Vector2f(0.f, -3.f);
//        line[i].color = sf::Color(255, 133, 77);
//    }
//    w.draw(line);
//}

void Rope::Draw(sf::RenderWindow& w)
{
    if (nodes.size() < 2) return;

    const float thickness = 0.5f;                   
    const sf::Color ropeCol = sf::Color(255, 133, 77);

    sf::VertexArray strip(sf::TriangleStrip);
    strip.resize(nodes.size() * 2);                  
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        sf::Vector2f p = nodes[i]->GetPosition() + sf::Vector2f(0.f, -3.f);

        sf::Vector2f dir;
        if (i == 0)              dir = nodes[1]->GetPosition() - nodes[0]->GetPosition();
        else if (i == nodes.size() - 1) dir = nodes[i]->GetPosition() - nodes[i - 1]->GetPosition();
        else                          dir = nodes[i + 1]->GetPosition() - nodes[i - 1]->GetPosition();

        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len == 0.f) len = 1.f;                    
        sf::Vector2f n(-dir.y / len, dir.x / len);        

        sf::Vector2f offset = n * (thickness * 0.5f);

        strip[i * 2].position = p + offset;
        strip[i * 2 + 1].position = p - offset;
        strip[i * 2].color = ropeCol;
        strip[i * 2 + 1].color = ropeCol;
    }

    w.draw(strip);
}