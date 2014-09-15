#include "stdafx.h"
#include "Human.h"
#include "Sound.h"
#include "Print.h"
#include "CustomDialogBox.h"
#include "GameManager.h"

Human::Human()
{
	m_PlayerType = HUMAN_PLAYER;
	m_ShipMoveLoop = ON;
	m_MyDialogBox = nullptr;
}


Human::~Human()
{
}

void Human::MoveShip(Ship* ship, Position& curPos, Direction& curDir)
{
	int maxHp = ship->GetMaxHP();
	int input = _getch();
	switch (input)
	{
	case SCAN_CODE:
		input = _getch();
		switch (input)
		{
		case UP_KEY:
			if (IsValidShipPosition(curPos.x, curPos.y - 1, maxHp, curDir))
				curPos.y--;
			break;
		case DOWN_KEY:
			if (IsValidShipPosition(curPos.x, curPos.y + 1, maxHp, curDir))
				curPos.y++;
			break;
		case LEFT_KEY:
			if (IsValidShipPosition(curPos.x - 1, curPos.y, maxHp, curDir))
				curPos.x--;
			break;
		case RIGHT_KEY:
			if (IsValidShipPosition(curPos.x + 1, curPos.y, maxHp, curDir))
				curPos.x++;
			break;
		}
		Sound::Instance().MenuMoveSound();
		break;
	case ENTER_KEY:
		m_ShipMoveLoop = OFF;
		Sound::Instance().MenuEnterSound();
		break;
	case SPACE_KEY:
		int temp = curDir;
		temp = (temp + 1) % 4;
		if (IsValidShipPosition(curPos.x, curPos.y, maxHp, (Direction)temp))
			curDir = (Direction)temp;
		Sound::Instance().MenuMoveSound();
		break;
	}
}

void Human::DeleteShip(Ship* ship, Position curPos, Direction curDir)
{
	if (ship == nullptr || curPos.x < 0 || curPos.y < 0)
		return;

	for (int i = 0; i < ship->GetMaxHP(); ++i)
	{
		m_MyBoard->AddPosition(curPos.x, curPos.y, NONE);

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

void Human::SettingShips()
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
		Position curPos = { startX, startY };

		while (m_ShipMoveLoop)
		{
			m_MyBoard->PrintBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y });
			m_MyBoard->UpdateBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y }, IAM);
			Print::Instance().PrintText();
			Print::Instance().Init();
			DeleteShip(ship, curPos, direction);
			MoveShip(ship, curPos, direction);
			PlaceShip(ship, curPos.x, curPos.y, direction);
		}
		m_ShipMoveLoop = ON;
	}
}

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

