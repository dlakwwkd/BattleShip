#pragma once
#include "ship.h"
class AirCraft : public Ship
{
public:
	AirCraft();
	~AirCraft();


	HitResult HitCheck(Position hitPos);
};

