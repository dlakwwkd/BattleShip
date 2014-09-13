#include "stdafx.h"
#include "Destroyer.h"


Destroyer::Destroyer()
{
	m_MaxHP = 2;
	m_HP = m_MaxHP;
	m_Type = DESTROYER;
	m_Name = "Destroyer";
	m_Pos.reserve(m_MaxHP);
	for (int i = 0; i < m_MaxHP; ++i)
	{
		m_Pos.push_back({ -1, -1 });
	}
}


Destroyer::~Destroyer()
{
}

HitResult Destroyer::HitCheck(POINT hitPos)
{
	HitResult hitResult = Ship::HitCheck(hitPos);

	return (hitResult == DESTROY) ? DESTROY_DESTROYER : hitResult;
}
