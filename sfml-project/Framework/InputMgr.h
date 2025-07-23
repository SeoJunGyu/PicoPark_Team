#pragma once

enum class Axis
{
	Horizontal,
	Vertical,
};

struct AxisInfo
{
	Axis axis;
	std::list<int> positives; // 1.f;
	std::list<int> negatives; // -1.f;

	float sensi = 10.f;
	float value = 0.f;
};

struct PlayerKeyMap
{
	std::list<int> posH;
	std::list<int> negH;
	sf::Keyboard::Key jump;
};

class InputMgr
{
private:
	static std::list<int> downKeys;
	static std::list<int> heldKeys;
	static std::list<int> upKeys;

	static std::unordered_map<Axis, AxisInfo> axisInfoMap;
	static sf::Vector2i mousePosition;

	static std::array<PlayerKeyMap, 4> keyMaps;

public:
	static void Init();
	static void Clear();

	static void UpdateEvent(const sf::Event& ev);
	static void Update(float dt);

	static bool GetKeyDown(sf::Keyboard::Key key);
	static bool GetKeyUp(sf::Keyboard::Key key);
	static bool GetKey(sf::Keyboard::Key key);

	static bool Contains(const std::list<int>& list, int key);
	static void Remove(std::list<int>& list, int key);

	static float GetAxisRaw(Axis axis); // -1, 0, 1
	static float GetAxis(Axis axis); // -1 ~ 1
	static float GetAxis(int idx, Axis axis); // -1 ~ 1

	static bool GetMouseButtonDown(sf::Mouse::Button key);
	static bool GetMouseButtonUp(sf::Mouse::Button key);
	static bool GetMouseButton(sf::Mouse::Button key);

	static sf::Vector2i GetMousePosition();

	static bool GetJump(int idx);
};

