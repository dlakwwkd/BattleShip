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
		//가장 높은 우선순위 값 검색
		for (int i = 0; i < maxHeight; ++i)
		{
			for (int j = 0; j < maxWidth; ++j)
			{
				if (m_PriorityPos[i][j] > highstPriority)
					highstPriority = m_PriorityPos[i][j];
			}
		}
		//검색된 우선순위 값에 해당하는 좌표 검색
		for (int i = 0; i < maxHeight; ++i)
		{
			for (int j = 0; j < maxWidth; ++j)
			{
				if (m_PriorityPos[i][j] == highstPriority)
				{
					//검색된 좌표가 공격 가능한 곳인지 확인
					if (!m_EnemyBoard->IsValidAttack(j, i))
					{
						//공격 불가능한 곳이면, 우선순위만 박탈
						m_PriorityPos[i][j] = -1;
					}
					else
					{
						//공격 가능한 곳이면, 우선순위 박탈 후 해당 좌표 반환
						m_PriorityPos[i][j] = -1;
						return{ j, i };
					}
				}
			}
		}
		//위의 반복문이 끝났다는 것은, 해당 우선순위의 좌표 중 공격 가능한 곳이 없다는 것
		//따라서 우선순위를 다시 검색한다.
	} while (highstPriority == -1);
	//highstPriority가 -1이라는 것은 공격할 곳이 아무데도 없다는 뜻
	return{ -1, -1 };
}
