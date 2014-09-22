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
	�÷��̾ �ʱ�ȭ �ϴ� �Լ�
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
	�� �÷��̾��� ��� �谡 ħ���ߴ��� Ȯ���ϴ� �Լ�
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
	���ݹ��� ��ǥ(pos)�� �޾� �� ����� ��ȯ�ϴ� �Լ�
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
	��(ship)�� �� ��������(startPos)�� ����(dir)�� �޾� �踦 ���� �Լ�
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
	��������(startPos)�� ����(dir)�� ���� ����(maxHp)�� �޾� �谡 ���� �� �ִ� ��ǥ���� Ȯ���ϴ� �Լ�
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
