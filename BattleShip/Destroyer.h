#pragma once
#include "ship.h"
class Destroyer : public Ship
{
public:
	Destroyer();
	~Destroyer();


	HitResult HitCheck(Position hitPos);
};

