#include "stdafx.h"
#include "AirCraft.h"


AirCraft::AirCraft()
{
	m_MaxHP = 5;
	m_HP = m_MaxHP;
	m_Type = AIRCRAFT;
	m_Name = "AirCraft";
	m_Pos.reserve(m_MaxHP);
	for (int i = 0; i < m_MaxHP; ++i)
	{
		m_Pos.push_back({ -1, -1 });
	}
}


AirCraft::~AirCraft()
{
}

HitResult AirCraft::HitCheck(POINT hitPos)
{
	HitResult hitResult = Ship::HitCheck(hitPos);

	return (hitResult == DESTROY) ? DESTROY_AIRCRAFT : hitResult;
}
