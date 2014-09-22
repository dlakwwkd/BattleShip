#include "stdafx.h"
#include "Ship.h"

Ship::Ship()
{
}


Ship::~Ship()
{
}

/*
	배의 상태를 초기화 하는 함수
*/
void Ship::InitShip()
{
	for (auto& pos : m_Pos)
	{
		pos.x = -1;
		pos.y = -1;
	}
	m_HP = m_MaxHP;
}

/*
	배의 한 부분의 좌표(pos)를 받아 맵에 놓는 함수
*/
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

/*
	배가 맞았는지를 체크하여 그 결과를 반환하는 함수
*/
HitResult Ship::HitCheck(Position hitPos)
{
	for (auto& shipPos : m_Pos)
	{
		if (shipPos.x == hitPos.x && shipPos.y == hitPos.y)
		{
			if (--m_HP == 0)
				return DESTROY;
			return HIT;
		}
	}
	return MISS;
}
