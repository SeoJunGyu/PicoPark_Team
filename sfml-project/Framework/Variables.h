#pragma once

class Player;
class Gimmick;

class Variables
{
public:
	static bool isDrawHitBox;

	static bool KeyObtained; //���� ���� ����
	static std::vector<Player*> players;
	static std::vector<Gimmick*> gimmicks;

	static void ResetStage();
};

