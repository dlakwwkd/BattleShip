#include "stdafx.h"
#include "Ship.h"

Ship::Ship()
{
}


Ship::~Ship()
{
}


void Ship::InitShip()
{
	for (auto& pos : m_Pos)
	{
		pos.x = -1;
		pos.y = -1;
	}
	m_HP = m_MaxHP;
}

void Ship::AddPosition(Position pos)
{
	for (auto& shipPos : m_Pos)
	{
		if (shipPos.x == pos.x && shipPos.y == pos.y)
		{
			printf_s("ERROR : Already Exist\n");
			return;
		}
	}
	for (auto& shipPos : m_Pos)
	{
		if (shipPos.x == -1)
		{
			shipPos.x = pos.x;
			shipPos.y = pos.y;
			break;
		}
	}
}

HitResult Ship::HitCheck(Position hitPos)
{
	for (auto& shipPos : m_Pos)
	{
		if (shipPos.x == hitPos.x && shipPos.y == hitPos.y)
		{
			if (--m_HP == 0)
			{
				return DESTROY;
			}
			return HIT;
		}
	}
	return MISS;
}
