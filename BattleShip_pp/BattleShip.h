#pragma once
#include "ship.h"
class BattleShip : public Ship
{
public:
	BattleShip();
	~BattleShip();


	HitResult HitCheck(Position hitPos);
};

