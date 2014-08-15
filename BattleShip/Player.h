#pragma once
#include "stdafx.h"
#include "Board.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Enums.h"
#include "Position.h"

class Player
{
public:
	Player();
	~Player();

	Board* GetMyBoard() { return m_MyBoard; }
	Board* GetEnemyBoard() { return m_EnemyBoard; }

	//void Submit();
	//void GiveUp();
	void SetupShips(); // random location, random direction, check if within map bounds, ships can't overlap
	void PrintShips();
	void SetEnemyBoard(Board* enemyBoard);
	void ProcessHitResult(HitResult hit);
	bool IsAllSunk();
	Position Attack();
	HitResult DoHitCheck(Position pos);
protected:
	void PlaceShip(Ship* ship, int startX, int startY, Direction direction);
	bool IsValidShipPosition(int startX, int startY, int maxHp, Direction direction);

private:
	std::vector<Ship*> m_ShipList;
	AirCraft*	m_Aircraft;
	BattleShip*	m_Battleship;
	Cruiser*	m_Cruiser;
	Destroyer*	m_Destroyer;

	Board* m_MyBoard;
	Board* m_EnemyBoard;
	int m_Type;
};

