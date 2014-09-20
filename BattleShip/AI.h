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
	void		RangeUpdate(int range, Position& pos, PriorityCalcRate crossForm, PriorityCalcRate xForm);
	void		DecideNextAttack(Position& pos, Direction dir);
	Direction	DecideBestDir(Position& pos);

	int**		m_PriorityPos;
	int			m_MaxShipSize;
	HitResult	m_PrevHitResult;
	Direction	m_PriorityDir;
	Position	m_FirstHitPos;
	Position	m_PrevHitPos;
};

