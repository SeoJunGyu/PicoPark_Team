#pragma once

class Player;

class Variables
{
public:
	static bool isDrawHitBox;

	static bool KeyObtained; //¿­¼è ½Àµæ ¿©ºÎ
	static std::vector<Player*> players;

	static void ResetStage();
};

