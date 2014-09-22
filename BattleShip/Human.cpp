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
	�����̷��� ��(ship)�� ���� ��ġ(curPos)�� ���� ����(curDir)�� �޾� �踦 �̵���Ű�� �Լ�
*/
void Human::MoveShip(Ship* ship, Position* curPos, Direction* curDir)
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
	�����Ϸ��� ��(ship)�� ���� ��ġ(curPos)�� ���� ����(curDir)�� �޾� �踦 �����ϴ� �Լ�
*/
void Human::DeleteShip(Ship* ship, Position curPos, Direction curDir)
{
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
	�踦 ��ġ�ϴ� �Լ�
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

		// �ϴ� ������ ��ġ�� �踦 ����
		do {
			direction = (Direction)(rand() % 4);
			startPos.x = rand() % maxWidth;
			startPos.y = rand() % maxHeight;
		} while (!IsValidShipPosition(startPos, direction, maxHp));
		
		PlaceShip(ship, startPos, direction);
		Position curPos = startPos;

		// �÷��̾ ENTER�� �Է��� ������ �̵����� ���
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
	�÷��̾��� �Է��� �޾� �ش� ��ǥ�� �����ϴ� �Լ�
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

