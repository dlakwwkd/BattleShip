#include "stdafx.h"
#include "Human.h"
#include "Print.h"
#include "Sound.h"
#include "GameManager.h"
#include "CustomDialogBox.h"


Human::Human()
{
	m_PlayerType	= HUMAN_PLAYER;
	m_ShipMoveLoop	= ON;
	m_MyDialogBox	= nullptr;
}


Human::~Human()
{
}

/*
	움직이려는 배(ship)와 현재 위치(curPos)와 현재 방향(curDir)을 받아 배를 이동시키는 함수
*/
void Human::MoveShip(Ship* ship, Position* curPos, Direction* curDir)
{
	_ASSERT(!(ship == nullptr || curPos == nullptr || curDir == nullptr));
	if (ship == nullptr || curPos == nullptr || curDir == nullptr)
		return;

	int maxHp = ship->GetMaxHP();
	int input = _getch();
	switch (input)
	{
	case SCAN_CODE:
		input = _getch();
		switch (input)
		{
		case UP_KEY:
			if (IsValidShipPosition({ curPos->x, curPos->y - 1 }, *curDir, maxHp))
				--curPos->y;
			break;
		case DOWN_KEY:
			if (IsValidShipPosition({ curPos->x, curPos->y + 1 }, *curDir, maxHp))
				++curPos->y;
			break;
		case LEFT_KEY:
			if (IsValidShipPosition({ curPos->x - 1, curPos->y }, *curDir, maxHp))
				--curPos->x;
			break;
		case RIGHT_KEY:
			if (IsValidShipPosition({ curPos->x + 1, curPos->y }, *curDir, maxHp))
				++curPos->x;
			break;
		}
		Sound::Instance().MenuMoveSound();
		break;
	case ENTER_KEY:
		m_ShipMoveLoop = OFF;
		Sound::Instance().MenuEnterSound();
		break;
	case SPACE_KEY:
		int temp = *curDir;
		temp = (temp + 1) % 4;
		if (IsValidShipPosition(*curPos, (Direction)temp, maxHp))
			*curDir = (Direction)temp;
		Sound::Instance().MenuMoveSound();
		break;
	}
}

/*
	제거하려는 배(ship)와 현재 위치(curPos)와 현재 방향(curDir)을 받아 배를 제거하는 함수
*/
void Human::DeleteShip(Ship* ship, Position curPos, Direction curDir)
{
	_ASSERT(!(ship == nullptr || curPos.x < 0 || curPos.y < 0));
	if (ship == nullptr || curPos.x < 0 || curPos.y < 0)
		return;

	for (int i = 0; i < ship->GetMaxHP(); ++i)
	{
		m_MyBoard->AddPosition(curPos, NONE_SHIP);

		switch (curDir)
		{
		case UP:	curPos.y--;	break;
		case DOWN:	curPos.y++;	break;
		case LEFT:	curPos.x--;	break;
		case RIGHT:	curPos.x++;	break;
		}
	}
	ship->InitShip();
}

/*
	배를 배치하는 함수
*/
void Human::SettingShips()
{
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();
	Position startPos = { 0, 0 };

	for (auto& ship : m_ShipList)
	{
		int maxHp = ship->GetMaxHP();
		Direction direction = Direction(UP);

		// 일단 임의의 위치에 배를 놓고
		do {
			direction = (Direction)(rand() % 4);
			startPos.x = rand() % maxWidth;
			startPos.y = rand() % maxHeight;
		} while (!IsValidShipPosition(startPos, direction, maxHp));
		
		PlaceShip(ship, startPos, direction);
		Position curPos = startPos;

		// 플레이어가 ENTER를 입력할 때까지 이동모드로 대기
		while (m_ShipMoveLoop)
		{
			m_MyBoard->PrintBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y });
			m_MyBoard->UpdateBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y }, IAM);
			Print::Instance().PrintText();
			Print::Instance().Init();
			DeleteShip(ship, curPos, direction);
			MoveShip(ship, &curPos, &direction);
			PlaceShip(ship, curPos, direction);
		}
		m_ShipMoveLoop = ON;
	}
}

/*
	플레이어의 입력을 받아 해당 좌표를 공격하는 함수
*/
Position Human::Attack()
{
	std::string msg;
	Position pos = { 0, 0 };
	int attackX = -1;
	int attackY = -1;

	do{
		m_MyDialogBox->InputDialog();
		m_MyDialogBox->ProcessMessage();
		if (GameManager::Instance().GetMainLoopStatus() == OFF)
			return{ -1, -1 };

		msg = m_MyDialogBox->GetMessage();
		if (msg.length() == 3)
		{
			attackX = msg[1] - CHAR_X1;
			attackY = toupper(msg[0]) - CHAR_Y1;
		}
		m_MyDialogBox->InitDialog();
		m_MyDialogBox->PrintDialog();

	} while (!m_EnemyBoard->IsValidAttack(attackX, attackY));

	pos.x = attackX;
	pos.y = attackY;

	return pos;
}

