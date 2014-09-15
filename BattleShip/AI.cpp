#include "stdafx.h"
#include "AI.h"


AI::AI()
{
	m_PlayerType = AI_PLAYER;
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();

	m_PriorityPos = new int*[maxHeight];
	for (int i = 0; i < maxHeight; ++i)
	{
		*(m_PriorityPos + i) = new int[maxWidth];
	}
	for (int i = 0; i < maxHeight; ++i)
	{
		for (int j = 0; j < maxWidth; ++j)
		{
			m_PriorityPos[i][j] = 0;
		}
	}
}


AI::~AI()
{
	for (int i = 0; i < m_MyBoard->GetMaxHeight(); ++i)
	{
		delete *(m_PriorityPos + i);
	}
	delete m_PriorityPos;
}

void AI::SettingShips()
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

Position AI::Attack()
{
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();

	int highstPriority = -1;
	do{
		highstPriority = -1;
		//���� ���� �켱���� �� �˻�
		for (int i = 0; i < maxHeight; ++i)
		{
			for (int j = 0; j < maxWidth; ++j)
			{
				if (m_PriorityPos[i][j] > highstPriority)
					highstPriority = m_PriorityPos[i][j];
			}
		}
		//�˻��� �켱���� ���� �ش��ϴ� ��ǥ �˻�
		for (int i = 0; i < maxHeight; ++i)
		{
			for (int j = 0; j < maxWidth; ++j)
			{
				if (m_PriorityPos[i][j] == highstPriority)
				{
					//�˻��� ��ǥ�� ���� ������ ������ Ȯ��
					if (!m_EnemyBoard->IsValidAttack(j, i))
					{
						//���� �Ұ����� ���̸�, �켱������ ��Ż
						m_PriorityPos[i][j] = -1;
					}
					else
					{
						//���� ������ ���̸�, �켱���� ��Ż �� �ش� ��ǥ ��ȯ
						m_PriorityPos[i][j] = -1;
						return{ j, i };
					}
				}
			}
		}
		//���� �ݺ����� �����ٴ� ����, �ش� �켱������ ��ǥ �� ���� ������ ���� ���ٴ� ��
		//���� �켱������ �ٽ� �˻��Ѵ�.
	} while (highstPriority == -1);
	//highstPriority�� -1�̶�� ���� ������ ���� �ƹ����� ���ٴ� ��
	return{ -1, -1 };
}
