#pragma once
#include "stdafx.h"
#include "Board.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Enums.h"
#include "Setting.h"

class Player
{
public:
	Player();
	~Player();

	void		SetPlayerName(std::string name) { m_Name = name; }
	std::string	GetPlayerName()	{ return m_Name; }
	Board*		GetMyBoard() { return m_MyBoard; }
	Board*		GetEnemyBoard() { return m_EnemyBoard; }

	void		SetupShips(); 
	void		PrintShips();
	void		SetEnemyBoard(Board* enemyBoard);
	void		ProcessHitResult(HitResult hit);
	bool		IsAllSunk();
	Position	Attack();
	HitResult	DoHitCheck(Position pos);
protected:
	void PlaceShip(Ship* ship, int startX, int startY, Direction direction);
	bool IsValidShipPosition(int startX, int startY, int maxHp, Direction direction);

private:
	std::string			m_Name;
	std::vector<Ship*>	m_ShipList;
	AirCraft*			m_Aircraft;
	BattleShip*			m_Battleship;
	Cruiser*			m_Cruiser;
	Destroyer*			m_Destroyer[2];
	Board*				m_MyBoard;
	Board*				m_EnemyBoard;
	int					m_Type;
};

