#include "stdafx.h"
#include "Variables.h"

bool Variables::isDrawHitBox = false;

bool Variables::KeyObtained = false;
std::vector<Player*> Variables::players{};

void Variables::ResetStage()
{
	KeyObtained = false;
	for (auto* p : players) delete p;
	players.clear();
}
