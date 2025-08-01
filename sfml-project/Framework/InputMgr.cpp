#include "stdafx.h"
#include "InputMgr.h"

std::list<int> InputMgr::downKeys;
std::list<int> InputMgr::heldKeys;
std::list<int> InputMgr::upKeys;

std::unordered_map<Axis, AxisInfo> InputMgr::axisInfoMap;

sf::Vector2i InputMgr::mousePosition;

std::array<PlayerKeyMap, 4> InputMgr::keyMaps;

int InputMgr::wheelDelta;

void InputMgr::Init()
{
	AxisInfo infoH;
	infoH.axis = Axis::Horizontal;
	infoH.positives.push_back(sf::Keyboard::D);
	infoH.positives.push_back(sf::Keyboard::Right);
	infoH.negatives.push_back(sf::Keyboard::A);
	infoH.negatives.push_back(sf::Keyboard::Left);
	axisInfoMap.insert({ Axis::Horizontal , infoH });
	AxisInfo infoV;
	infoV.axis = Axis::Vertical;
	infoV.positives.push_back(sf::Keyboard::S);
	infoV.positives.push_back(sf::Keyboard::Down);
	infoV.negatives.push_back(sf::Keyboard::W);
	infoV.negatives.push_back(sf::Keyboard::Up);
	axisInfoMap.insert({ Axis::Vertical , infoV });

	keyMaps[0].posH = { sf::Keyboard::D };
	keyMaps[0].negH = { sf::Keyboard::A };
	keyMaps[0].jump = sf::Keyboard::W;

	keyMaps[1].posH = { sf::Keyboard::L };
	keyMaps[1].negH = { sf::Keyboard::J };
	keyMaps[1].jump = sf::Keyboard::I;

	keyMaps[2].posH = { sf::Keyboard::Right };
	keyMaps[2].negH = { sf::Keyboard::Left };
	keyMaps[2].jump = sf::Keyboard::Up;

	keyMaps[3].posH = { sf::Keyboard::Numpad6 };
	keyMaps[3].negH = { sf::Keyboard::Numpad4 };
	keyMaps[3].jump = sf::Keyboard::Numpad8;
}

void InputMgr::Clear() 
{
	downKeys.clear();
	upKeys.clear();
	wheelDelta = 0;
}

int InputMgr::PopWheelDelta()
{
	int d = wheelDelta;
	wheelDelta = 0;               
	return d;
}

void InputMgr::UpdateEvent(const sf::Event& ev) 
{
	switch (ev.type)
	{
	case sf::Event::KeyPressed:
		if (!Contains(heldKeys, ev.key.code))
		{
			downKeys.push_back(ev.key.code);
			heldKeys.push_back(ev.key.code);
		}
		break;
	case sf::Event::KeyReleased:
		Remove(heldKeys, ev.key.code);
		upKeys.push_back(ev.key.code);
		break;
	case sf::Event::MouseButtonPressed:
	{
		int code = sf::Keyboard::KeyCount + ev.mouseButton.button;
		if (!Contains(heldKeys, code))
		{
			downKeys.push_back(code);
			heldKeys.push_back(code);
		}
	}
		break;
	case sf::Event::MouseWheelScrolled:
		if (ev.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
			wheelDelta += static_cast<int>(ev.mouseWheelScroll.delta);
		break;
	case sf::Event::MouseButtonReleased:
	{
		int code = sf::Keyboard::KeyCount + ev.mouseButton.button;
		Remove(heldKeys, code);
		upKeys.push_back(code);
	}
		break;
	}
}

void InputMgr::Update(float dt) 
{
	mousePosition = sf::Mouse::getPosition(FRAMEWORK.GetWindow());

	for (auto& pair : axisInfoMap)
	{
		AxisInfo& axisInfo = pair.second;
		float raw = GetAxisRaw(axisInfo.axis); // -1, 0, 1
		float dir = raw;
		if (raw == 0.f && axisInfo.value != 0.f)
		{
			dir = axisInfo.value > 0.f ? -1.f : 1.f;
		}
		axisInfo.value += dir * axisInfo.sensi * dt;
		axisInfo.value = Utils::Clamp(axisInfo.value, -1.f, 1.f);

		float stopThreshold = std::abs(dir * axisInfo.sensi * dt);
		if (raw == 0.f && std::abs(axisInfo.value) < stopThreshold)
		{
			axisInfo.value = 0.f;
		}
	}

}

bool InputMgr::GetKeyDown(sf::Keyboard::Key key)
{
	return Contains(downKeys, key);
}

bool InputMgr::GetKeyUp(sf::Keyboard::Key key)
{
	return Contains(upKeys, key);
}

bool InputMgr::GetKey(sf::Keyboard::Key key)
{
	return Contains(heldKeys, key);
}

bool InputMgr::Contains(const std::list<int>& list, int key)
{
	return std::find(list.begin(), list.end(), key) != list.end();
}

void InputMgr::Remove(std::list<int>& list, int key)
{
	list.remove(key);
}

float InputMgr::GetAxisRaw(Axis axis)
{
	auto findIt = axisInfoMap.find(axis);
	if (findIt == axisInfoMap.end())
		return 0.f;

	const AxisInfo& axisInfo = findIt->second;
	auto it = heldKeys.rbegin();
	while (it != heldKeys.rend())
	{
		int code = *it;
		if (Contains(axisInfo.positives, code))
		{
			return 1.f;
		}
		if (Contains(axisInfo.negatives, code))
		{
			return -1.f;
		}
		++it;
	}
	return 0.0f;
}

float InputMgr::GetAxis(Axis axis)
{
	auto findIt = axisInfoMap.find(axis);
	if (findIt == axisInfoMap.end())
	{
		return 0.0f;
	}
	return findIt->second.value;
}

float InputMgr::GetAxis(int idx, Axis axis)
{
	auto& keymap = keyMaps[idx];
	const std::list<int>& pos = (axis==Axis::Horizontal) ? keymap.posH : std::list<int>();
	const std::list<int>& neg = (axis==Axis::Horizontal) ? keymap.negH : std::list<int>();

	for (auto it = heldKeys.rbegin(); it != heldKeys.rend(); it++)
	{
		int code = *it;
		if (Contains(pos, code))
		{
			return 1.f;
		}
		if (Contains(neg, code))
		{
			return -1.f;
		}
	}

	return 0.0f;
}

bool InputMgr::GetMouseButtonDown(sf::Mouse::Button key)
{
	return Contains(downKeys, sf::Keyboard::KeyCount + key);;
}

bool InputMgr::GetMouseButtonUp(sf::Mouse::Button key)
{
	return Contains(upKeys, sf::Keyboard::KeyCount + key);;
}

bool InputMgr::GetMouseButton(sf::Mouse::Button key)
{
	return Contains(heldKeys, sf::Keyboard::KeyCount + key);;
}

sf::Vector2i InputMgr::GetMousePosition()
{
	return mousePosition; 
}

bool InputMgr::GetJump(int idx)
{
	return GetKeyDown(keyMaps[idx].jump);
}

