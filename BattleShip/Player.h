#pragma once
#include "stdafx.h"
#include "Board.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Enums.h"

#define MAX_PREV_INFO_NUM 5

class Player
{
public:
	Player();
	~Player();

	void				SetPlayerName(std::string name) { m_Name = name; }
	void				SetEnemyBoard(Board* enemyBoard) { m_EnemyBoard = enemyBoard; }
	std::string			GetPlayerName()	{ return m_Name; }
	Board*				GetMyBoard() { return m_MyBoard; }
	Board*				GetEnemyBoard() { return m_EnemyBoard; }
	PlayerType			GetPlayerType() { return m_PlayerType; }
	std::vector<Ship*>*	GetShipList(){ return &m_ShipList; }

	bool				IsAllSunk();
	HitResult			DoHitCheck(Position pos);

	virtual void		InitPlayer();
	virtual void		SettingShips() = 0;
	virtual Position	Attack() = 0;

protected:
	void PlaceShip(Ship* ship, int startX, int startY, Direction direction);
	bool IsValidShipPosition(int startX, int startY, int maxHp, Direction direction);

protected:
	PlayerType				m_PlayerType;
	std::string				m_Name;
	std::vector<Ship*>		m_ShipList;
	Board*					m_MyBoard;
	Board*					m_EnemyBoard;
};

