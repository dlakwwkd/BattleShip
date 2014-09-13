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

void Ship::AddPosition(POINT pos)
{
	// 배의 일부분이 지정된 위치에 이미 있는지 체크
	for (auto& shipPos : m_Pos)
	{
		if (shipPos.x == pos.x && shipPos.y == pos.y)
		{
			printf_s("ERROR : Already Exist\n");
			return;
		}
	}
	// 아직 놓이지 않은 배의 부분을 지정된 위치에 놓음
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

HitResult Ship::HitCheck(POINT hitPos)
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
