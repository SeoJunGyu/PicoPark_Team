#pragma once

class Player;

class Variables
{
public:
	static bool isDrawHitBox;

	static bool KeyObtained; //���� ���� ����
	static std::vector<Player*> players;

	static void ResetStage();
};

