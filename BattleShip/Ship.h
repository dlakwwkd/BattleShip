#pragma once
#include "Enums.h"
#include "Position.h"

class Ship
{
public:
	Ship();
	~Ship();

	void AddPosition(char x, char y);
	void AddPosition(Position pos);
	void Print();
	bool PositionCheck(int posX, int posY);
	int GetHP() { return m_HP; }
	int GetMaxHP() { return m_MaxHP; }
	ShipType GetShipType() { return m_Type; }
	virtual HitResult HitCheck(Position hitPos);
	
protected:
	std::string m_Name;
	ShipType m_Type;
	Position m_Pos[5];
	int m_HP;
	int m_MaxHP;

};