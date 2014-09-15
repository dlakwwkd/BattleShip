#pragma once
#include "Player.h"

class CustomDialogBox;

class Human :
	public Player
{
public:
	Human();
	~Human();

	void		SetMyDialogBox(CustomDialogBox* box) { m_MyDialogBox = box; }

	void		MoveShip(Ship* ship, Position& curPos, Direction& curDir);
	void		DeleteShip(Ship* ship, Position curPos, Direction curDir);
	void		SettingShips();
	Position	Attack();

private:
	OnOff				m_ShipMoveLoop;
	CustomDialogBox*	m_MyDialogBox;
};

