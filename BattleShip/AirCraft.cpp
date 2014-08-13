#include "stdafx.h"
#include "AirCraft.h"


AirCraft::AirCraft()
{
	m_MaxHP = 5;
	m_HP = m_MaxHP;
	m_Type = AIRCRAFT;
	m_Name = "AirCraft";
}


AirCraft::~AirCraft()
{
}

HitResult AirCraft::HitCheck(Position hitPos)
{
	HitResult hitResult = Ship::HitCheck(hitPos);

	return (hitResult == DESTROY) ? DESTROY_AIRCRAFT : hitResult;
}
