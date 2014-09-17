#include "stdafx.h"
#include "Player.h"
#include "Sound.h"

Player::Player()
{
	m_ShipList.reserve(5);
	m_ShipList.push_back(new AirCraft());
	m_ShipList.push_back(new BattleShip());
	m_ShipList.push_back(new Cruiser());
	m_ShipList.push_back(new Destroyer());
	m_ShipList.push_back(new Destroyer());

	m_MyBoard		= new Board();
	m_EnemyBoard	= nullptr;
	m_Name			= "name";

	m_MyBoard->SetBoardName(m_Name);	
}


Player::~Player()
{
	for (auto& ship : m_ShipList)
	{
		delete ship;
	}
	m_ShipList.clear();

	delete m_MyBoard;
}


void Player::InitPlayer()
{
	for (auto& ship : m_ShipList)
	{
		ship->InitShip();
	}
	m_MyBoard->InitBoard();
	m_EnemyBoard = nullptr;
}

bool Player::IsAllSunk()
{
	for (auto& ship : m_ShipList)
	{
		if (ship->GetHP() != 0)
			return false;
	}
	return true;
}

HitResult Player::DoHitCheck(Position pos)
{
	for (auto& ship : m_ShipList)
	{
		HitResult hitResult = ship->HitCheck(pos);
		
		switch (hitResult)
		{
		case MISS:
			break;
		case HIT:
			return hitResult;
		default:
			m_MyBoard->ProcessDestroy(ship->GetShipPos());
			return hitResult;
		}
	}
	return MISS;
}

void Player::PlaceShip(Ship* ship, int startX, int startY, Direction direction)
{
	if (ship == nullptr || startX < 0 || startY < 0)
		return;

	int curX = startX;
	int curY = startY;

	for (int i = 0; i < ship->GetMaxHP(); ++i)
	{
		ship->AddPosition({ curX, curY });
		m_MyBoard->AddPosition(curX, curY, ship->GetShipType());

		switch (direction)
		{
		case UP:	curY--;	break;
		case DOWN:	curY++;	break;
		case LEFT:	curX--;	break;
		case RIGHT:	curX++;	break;
		}
	}
}

bool Player::IsValidShipPosition(int startX, int startY, int maxHp, Direction direction)
{
	if (maxHp < 1) return false;

	int curX = startX;
	int curY = startY;

	for (int i = 0; i < maxHp; ++i)
	{
		if (m_MyBoard->MapCheck(curX, curY) == false)
		{
			return false;
		}
		if (m_MyBoard->IsShipHere(curX, curY))
		{
			return false;
		}

		switch (direction)
		{
		case UP:	curY--;	break;
		case DOWN:	curY++;	break;
		case LEFT:	curX--;	break;
		case RIGHT:	curX++;	break;
		}
	}
	return true;
}
