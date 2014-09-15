#pragma once
#include "Player.h"


class AI :
	public Player
{
public:
	AI();
	~AI();
 
 	//void		PriorityFix(Position pos, HitResult hit);
	void		SettingShips();
	Position	Attack();

private:
	int**		m_PriorityPos;
};

