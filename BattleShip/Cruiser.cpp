#include "stdafx.h"
#include "Cruiser.h"


Cruiser::Cruiser()
{
	m_MaxHP = 3;
	m_HP = m_MaxHP;
	m_Type = CRUISER;
	m_Name = "Cruiser";
	m_Pos.reserve(m_MaxHP);
	for (int i = 0; i < m_MaxHP; ++i)
	{
		m_Pos.push_back({ -1, -1 });
	}
}


Cruiser::~Cruiser()
{
}

HitResult Cruiser::HitCheck(POINT hitPos)
{
	HitResult hitResult = Ship::HitCheck(hitPos);

	return (hitResult == DESTROY) ? DESTROY_CRUISER : hitResult;
	
}
