#include "stdafx.h"
#include "BattleShip.h"


BattleShip::BattleShip()
{
	m_MaxHP = 4;
	m_HP = m_MaxHP;
	m_Type = BATTLESHIP;
	m_Name = "BattleShip";
	m_Pos.reserve(m_MaxHP);
	for (int i = 0; i < m_MaxHP; ++i)
	{
		m_Pos.push_back({ -1, -1 });
	}
}


BattleShip::~BattleShip()
{
}

HitResult BattleShip::HitCheck(Position hitPos)
{
	HitResult hitResult = Ship::HitCheck(hitPos);

	return (hitResult == DESTROY) ? DESTROY_BATTLESHIP : hitResult;
}