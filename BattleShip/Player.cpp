#include "stdafx.h"
#include "Player.h"
#include "Sound.h"

Player::Player()
{
	m_Aircraft		= new AirCraft();
	m_Battleship	= new BattleShip();
	m_Cruiser		= new Cruiser();
	m_Destroyer[0]	= new Destroyer();
	m_Destroyer[1]  = new Destroyer();

	m_ShipList.reserve(5);
	m_ShipList.push_back(m_Aircraft);
	m_ShipList.push_back(m_Battleship);
	m_ShipList.push_back(m_Cruiser);
	m_ShipList.push_back(m_Destroyer[0]);
	m_ShipList.push_back(m_Destroyer[1]);

	m_MyBoard		= new Board();
	m_EnemyBoard	= nullptr;
	m_Name			= "name";
	m_PrevAttackInfoList.reserve(MAX_PREV_INFO_NUM);

	m_MyBoard->SetBoardName(m_Name);	//플레이어 넘버 받아서 표시해야할 듯
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

void Player::SettingShips()
{
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();
	int startX = 0;
	int startY = 0;

	for (auto& ship : m_ShipList)
	{
		int maxHp = ship->GetMaxHP();
		Direction direction = Direction(UP);

		do {
			direction = (Direction)(rand() % 4);
			startX = rand() % maxWidth;
			startY = rand() % maxHeight;
		} while (!IsValidShipPosition(startX, startY, maxHp, direction));

		PlaceShip(ship, startX, startY, direction);
	}
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

POINT Player::Attack()
{
	if (m_PrevAttackInfoList.empty()) return{ -1, -1 };

	POINT pos = { 0, 0 };
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();
	int attackX = 0;
	int attackY = 0;

	do{
// 		if (m_PrevAttackInfoList.back().hit == HIT)
// 		{
// // 			attackX = ;
// // 			attackY = ;
// 		}
// 		else
// 		{
			attackX = rand() % maxWidth;
			attackY = rand() % maxHeight;
/*		}*/
	} while (!m_EnemyBoard->IsValidAttack(attackX, attackY));

	pos.x = attackX;
	pos.y = attackY;

	return pos;
}

HitResult Player::DoHitCheck(POINT pos)
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
