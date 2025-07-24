#include "stdafx.h"
#include "Variables.h"

bool Variables::isDrawHitBox = false;

bool Variables::KeyObtained = false;
std::vector<Player*> Variables::players{};
std::vector<Gimmick*> Variables::gimmicks{};

void Variables::ResetStage()
{
	KeyObtained = false;

	for (auto* g : gimmicks) delete g;
	gimmicks.clear();
	for (auto* p : players) delete p;
	players.clear();

	
}
