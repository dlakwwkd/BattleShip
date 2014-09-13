#pragma once
#include "Enums.h"
class Ship
{
public:
	Ship();
	~Ship();

	int					GetHP() { return m_HP; }
	int					GetMaxHP() { return m_MaxHP; }
	ShipType			GetShipType() { return m_Type; }
	std::vector<POINT>	GetShipPos() { return m_Pos; }

	void				InitShip();
	void				AddPosition(POINT pos);
	virtual HitResult	HitCheck(POINT hitPos);
	
protected:
	std::vector<POINT>	m_Pos;
	std::string			m_Name;
	ShipType			m_Type;
	int					m_HP;
	int					m_MaxHP;
};
