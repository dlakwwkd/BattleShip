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

/*
	플레이어를 초기화 하는 함수
*/
void Player::InitPlayer()
{
	for (auto& ship : m_ShipList)
	{
		ship->InitShip();
	}
	m_MyBoard->InitBoard();
	m_EnemyBoard = nullptr;
}

/*
	이 플레이어의 모든 배가 침몰했는지 확인하는 함수
*/
bool Player::IsAllSunk()
{
	for (auto& ship : m_ShipList)
	{
		if (ship->GetHP() != 0)
			return false;
	}
	return true;
}

/*
	공격받은 좌표(pos)를 받아 그 결과를 반환하는 함수
*/
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

/*
	배(ship)와 그 시작지점(startPos)과 방향(dir)을 받아 배를 놓는 함수
*/
void Player::PlaceShip(Ship* ship, Position startPos, Direction dir)
{
	if (ship == nullptr || startPos.x < 0 || startPos.y < 0)
		return;

	Position curPos = startPos;

	for (int i = 0; i < ship->GetMaxHP(); ++i)
	{
		ship->AddPosition(curPos);
		m_MyBoard->AddPosition(curPos, ship->GetShipType());

		switch (dir)
		{
		case UP:	curPos.y--;	break;
		case DOWN:	curPos.y++; break;
		case LEFT:	curPos.x--;	break;
		case RIGHT:	curPos.x++; break;
		}
	}
}

/*
	시작지점(startPos)와 방향(dir)과 배의 길이(maxHp)를 받아 배가 놓일 수 있는 좌표인지 확인하는 함수
*/
bool Player::IsValidShipPosition(Position startPos, Direction dir, int maxHp)
{
	if (maxHp < 1) return false;

	int curX = startPos.x;
	int curY = startPos.y;

	for (int i = 0; i < maxHp; ++i)
	{
		if (m_MyBoard->IsInBoard(curX, curY) == false)
			return false;
		if (m_MyBoard->IsShipHere(curX, curY))
			return false;

		switch (dir)
		{
		case UP:	curY--;	break;
		case DOWN:	curY++;	break;
		case LEFT:	curX--;	break;
		case RIGHT:	curX++;	break;
		}
	}
	return true;
}
