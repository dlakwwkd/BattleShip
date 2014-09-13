#pragma once
#include "stdafx.h"
#include "Board.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Enums.h"
#include "Setting.h"

#define MAX_PREV_INFO_NUM 5

class Player
{
public:
	Player();
	~Player();

	void		SetPlayerName(std::string name) { m_Name = name; }
	void		SetEnemyBoard(Board* enemyBoard) { m_EnemyBoard = enemyBoard; }
	void		SetPrevAttackInfo(AttackInfo Info) { m_PrevAttackInfoList.push_back(Info); }
	std::string	GetPlayerName()	{ return m_Name; }
	Board*		GetMyBoard() { return m_MyBoard; }
	Board*		GetEnemyBoard() { return m_EnemyBoard; }

	void		InitPlayer();
	void		SettingShips(); 
	bool		IsAllSunk();
	POINT	Attack();
	HitResult	DoHitCheck(POINT pos);
protected:
	void PlaceShip(Ship* ship, int startX, int startY, Direction direction);
	bool IsValidShipPosition(int startX, int startY, int maxHp, Direction direction);

protected:
	std::string				m_Name;
	std::vector<Ship*>		m_ShipList;
	AirCraft*				m_Aircraft;
	BattleShip*				m_Battleship;
	Cruiser*				m_Cruiser;
	Destroyer*				m_Destroyer[2];
	Board*					m_MyBoard;
	Board*					m_EnemyBoard;
	std::vector<AttackInfo>	m_PrevAttackInfoList;

};

