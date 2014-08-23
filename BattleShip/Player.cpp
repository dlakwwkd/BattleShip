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
	m_Type			= 0;

	m_MyBoard->SetBoardName(m_Name);	//플레이어 넘버 받아서 표시해야할 듯
// 	if (name == "1 Player")
// 		m_MyBoard->SetBoardPos({ 30, 5 });
// 	else
// 		m_MyBoard->SetBoardPos({ 60, 5 });

}


Player::~Player()
{
	for (auto& i : m_ShipList)
	{
		delete i;
	}
	delete m_MyBoard;
}

void Player::SetupShips()
{
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();
	int startX = 0;
	int startY = 0;

	for (unsigned int i = 0; i < m_ShipList.size(); ++i)
	{
		Ship *ship = m_ShipList[i];

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

void Player::PrintShips()
{
	for (auto& i : m_ShipList)
	{
		i->Print();
	}
}

void Player::SetEnemyBoard(Board* enemyBoard)
{
	m_EnemyBoard = enemyBoard;
}


void Player::ProcessHitResult(HitResult hit)
{
	
}

Position Player::Attack()
{
	Position pos;
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();

	do{
		pos.m_X = (char)(rand() % maxWidth);
		pos.m_Y = (char)(rand() % maxHeight);

	} while (!m_EnemyBoard->IsValidAttack(pos.m_X, pos.m_Y));

	return pos;
}

HitResult Player::DoHitCheck(Position pos)
{
	for (unsigned int i = 0; i < m_ShipList.size(); ++i)
	{
		HitResult hitResult = m_ShipList[i]->HitCheck(pos);
		
		if (hitResult != MISS)
			return hitResult;
	}
	return MISS;
}

bool Player::IsAllSunk()
{
	for (auto& i : m_ShipList)
	{
		if (i->GetHP() != 0)
			return false;
	}
	return true;
}

void Player::PlaceShip(Ship* ship, int startX, int startY, Direction direction)
{
	if (ship == nullptr || startX < 0 || startY < 0)
		return;

	for (int i = 0; i < ship->GetMaxHP(); ++i)
	{
		char curX = (char)('a' + startX);
		char curY = (char)('1' + startY);

		ship->AddPosition(curX, curY);
		m_MyBoard->AddPosition(startX, startY, (int)(ship->GetShipType()));

		switch (direction)
		{
		case UP:	startY--;	break;
		case DOWN:	startY++;	break;
		case LEFT:	startX--;	break;
		case RIGHT:	startX++;	break;
		}
	}
}

bool Player::IsValidShipPosition(int startX, int startY, int maxHp, Direction direction)
{
	if (maxHp < 1) return false;
	for (int i = 0; i < maxHp; ++i)
	{
		if (m_MyBoard->MapCheck(startX, startY) == false)
		{
			return false;
		}
		if (m_MyBoard->IsShipHere(startX, startY))
		{
			return false;
		}

		switch (direction)
		{
		case UP:	startY--;	break;
		case DOWN:	startY++;	break;
		case LEFT:	startX--;	break;
		case RIGHT:	startX++;	break;
		}
	}
	return true;
}
