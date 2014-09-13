#pragma once
#include "Enums.h"
#include "Setting.h"


class Ship
{
public:
	Ship();
	~Ship();

	int					GetHP() { return m_HP; }
	int					GetMaxHP() { return m_MaxHP; }
	ShipType			GetShipType() { return m_Type; }

	void				InitShip();
	void				AddPosition(Position pos);
	virtual HitResult	HitCheck(Position hitPos);
	
protected:
	std::string m_Name;
	ShipType	m_Type;
	Position	m_Pos[5];
	int			m_HP;
	int			m_MaxHP;
};
