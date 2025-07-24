#pragma once

class Player;
class Gimmick;

class Variables
{
public:
	static bool isDrawHitBox;

	static bool KeyObtained; //¿­¼è ½Àµæ ¿©ºÎ
	static std::vector<Player*> players;
	static std::vector<Gimmick*> gimmicks;

	static void ResetStage();
};

