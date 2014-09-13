#include "stdafx.h"
#include "Ship.h"

Ship::Ship()
{
	m_HP = 0;
	memset(m_Pos, 0, sizeof(Position)* 5);
}


Ship::~Ship()
{
}


void Ship::InitShip()
{
	for (int i = 0; i < m_MaxHP; ++i)
	{
		m_Pos[i].x = 0;
		m_Pos[i].y = 0;
	}
	m_HP = m_MaxHP;
}

void Ship::AddPosition(Position pos)
{
	for (int i = 0; i < m_HP; ++i)
	{
		if (m_Pos[i].x == pos.x && m_Pos[i].y == pos.y)
		{
			printf_s("ERROR : Already Exist\n");
			return;
		}
	}
	for (int i = 0; i < m_HP; ++i)
	{
		if (m_Pos[i].x == 0)
		{
			m_Pos[i].x = pos.x;
			m_Pos[i].y = pos.y;
			break;
		}
	}
}

HitResult Ship::HitCheck(Position hitPos)
{
	for (int i = 0; i < m_MaxHP; ++i)
	{
		if (m_Pos[i].x == hitPos.x && m_Pos[i].y == hitPos.y)
		{
			m_Pos[i].x = 0;
			m_Pos[i].y = 0;
			--m_HP;

			if (m_HP == 0) return DESTROY;
			return HIT;
		}
	}
	return MISS;
}
