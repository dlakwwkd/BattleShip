#pragma once
#include "Player.h"

class AI :
	public Player
{
public:
	AI();
	~AI();

	void		InitPlayer();
	void		InitPriority();
 	void		UpdatePriority(Position pos, HitResult hit);
	void		SettingShips();
	Position	Attack();

private:
	int**		m_PriorityPos;
	int			m_PossibleShipLength;
	HitResult	m_PrevHitResult;
	Direction	m_PriorityDir;
	Position	m_FirstHitPos;
	Position	m_PrevHitPos;
};

