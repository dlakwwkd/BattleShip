#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	m_Aircraft		= new AirCraft();
	m_Battleship	= new BattleShip();
	m_Cruiser		= new Cruiser();
	m_Destroyer		= new Destroyer();

	m_ShipList.reserve(5);
	m_ShipList.push_back(m_Aircraft);
	m_ShipList.push_back(m_Battleship);
	m_ShipList.push_back(m_Cruiser);
	m_ShipList.push_back(m_Destroyer);
	m_ShipList.push_back(m_Destroyer);

	m_MyBoard		= new Board();
	m_EnemyBoard	= new Board();

	m_MyBoard->SetBoardName("MyBoard");
	m_EnemyBoard->SetBoardName("Enemy's Board");
}


Player::~Player()
{
	for (auto& i : m_ShipList)
	{
		delete i;
	}
	delete m_MyBoard;
	delete m_EnemyBoard;
}

void Player::SetupShips()
{
	// Set initial variables
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();
	int startX = 0;
	int startY = 0;

	// Place each ship in shiplist
	for (int i = 0; i < (int)m_ShipList.size(); i++)
	{
		// Get ship reference
		Ship *ship = m_ShipList[i];


		// Set intial values
		int maxHp = ship->GetMaxHP();
		Direction direction = (Direction)UP;


		// random direction & start position generation for the ship
		do {

			direction = (Direction)(rand() % 4);
			startX = rand() % maxWidth;
			startY = rand() % maxHeight;

		} while (!IsValidShipPosition(startX, startY, maxHp, direction));	// boundary check, collision check


		// ��ġ
		PlaceShip(ship, startX, startY, direction);

	} // continue for loop 

	m_MyBoard->PrintBoard();
}

void Player::PrintShips()
{
	for (auto& i : m_ShipList)
	{
		i->Print();
	}
}

void Player::ProcessHitResult(HitResult hit)
{
	
}

Position Player::Attack()
{
	Position pos;
	pos.m_X = 'A';
	pos.m_Y = '1';

	return pos;
}

HitResult Player::DoHitCheck(Position pos)
{

	for (int i = 0; i < (int)m_ShipList.size(); i++)
	{
		// Get ship reference
		Ship *ship = m_ShipList[i];

		HitResult hitResult = ship->HitCheck(pos);
		
		if (hitResult != MISS)
		{
			return hitResult;
		}
	}

	return MISS;
}

void Player::PlaceShip(Ship* ship, int startX, int startY, Direction direction)
{
	// Iterate by amount of ship Max HP
	for (int i = 0; i < ship->GetMaxHP(); i++)
	{

		// Convert integer coordinates to characters
		char curX = (char)('a' + startX);
		char curY = (char)('1' + startY);


		// add position data to ships and to board
		ship->AddPosition(curX, curY);
		m_MyBoard->AddPosition(startX, startY, (int)(ship->GetShipType()));


		// increment coordinates based on direction
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
	// iterate by Max HP of ship
	for (int i = 0; i < maxHp; i++)
	{
		// Map Boundary Check
		if (!m_MyBoard->MapCheck(startX, startY))
		{
			return false;
		}


		// Ship Collision Check
		if (m_MyBoard->IsShipHere(startX, startY))
		{
			return false;
		}


		// Direction Increment
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
