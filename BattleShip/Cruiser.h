#pragma once
#include "ship.h"
class Cruiser : public Ship
{
public:
	Cruiser();
	~Cruiser();


	HitResult HitCheck(Position hitPos);
};

