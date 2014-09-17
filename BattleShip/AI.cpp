#include "stdafx.h"
#include "AI.h"


AI::AI()
{
	m_PlayerType = AI_PLAYER;
	m_PossibleShipLength = 5;
	m_PrevHitResult = NO_RESULT;
	m_PriorityDir = DOWN;
	m_FirstHitPos = { -1, -1 };
	m_PrevHitPos = { -1, -1 };

	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();

	m_PriorityPos = new int*[maxHeight];
	for (int i = 0; i < maxHeight; ++i)
	{
		m_PriorityPos[i] = new int[maxWidth];
	}
	for (int i = 0; i < maxHeight; ++i)
	{
		for (int j = 0; j < maxWidth; ++j)
		{
			m_PriorityPos[i][j] = 0;
		}
	}
	InitPriority();
}


AI::~AI()
{
	for (int i = 0; i < m_MyBoard->GetMaxHeight(); ++i)
	{
		delete m_PriorityPos[i];
	}
	delete m_PriorityPos;
}

void AI::InitPlayer()
{
	Player::InitPlayer();
	InitPriority();
}

void AI::InitPriority()
{
	m_PossibleShipLength = 5;
	m_PrevHitResult = NO_RESULT;
	m_PriorityDir = DOWN;
	m_FirstHitPos = { -1, -1 };
	m_PrevHitPos = { -1, -1 };
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();

	for (int i = 0; i < maxHeight; ++i)
	{
		for (int j = 0; j < maxWidth; ++j)
		{
			// 모든 배는 연속된 타일에 놓여있으므로, 기본적으로 한 종류 타일에만 우선순위를 좀 더 부여
			if ((i + j) % 2 == 0)
				m_PriorityPos[i][j] = NORMAL_INCREASE;
			else
				m_PriorityPos[i][j] = 0;

			// 그 중에서도 효율적으로 맵을 훑을 수 도록 여러번 추가 우선순위를 부여
			if ((i + j) % 4 == 2)
				m_PriorityPos[i][j] += LITTLE_INCREASE;
		}
	}
}

void AI::UpdatePriority(Position pos, HitResult hit)
{
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();

	switch (hit)
	{
		// 이번 공격이 히트일때,
	case HIT:
		// 먼저 현재 히트 좌표 주변의 우선순위를 높힌다.
		for (int i = -1; i < 2; ++i)
		{
			for (int j = -1; j < 2; ++j)
			{
				if ((pos.y + i) < 0 || (pos.y + i) >= maxHeight ||
					(pos.x + j) < 0 || (pos.x + j) >= maxWidth) break;

				if ((i + j) % 2 != 0)
					m_PriorityPos[pos.y + i][pos.x + j] += HIGH_INCREASE;
			}
		}
		// 저번 결과가 히트가 아니면 두 가지 경우의 수를 생각한다.
		if (m_PrevHitResult != HIT)
		{
			// 이번이 첫 히트이면, 주변 네 방향 중 가능성이 높은 곳을 계산하고 그쪽으로 진격한다.
			if (m_FirstHitPos.x == -1)
			{
				m_FirstHitPos = pos;

				int collectedPriority = 0;
				int temp = 0;
				// 4가지 방향에 대한 가능한 배 길이만큼의 우선순위를 수집
				for (int i = UP; i <= LEFT; ++i)
				{
					temp = 0;
					for (int j = 0; j < m_PossibleShipLength; ++j)
					{
						switch (i)
						{
						case UP:
							// 보드판을 벗어나면 우선순위 감소;
							if ((pos.y - j) < 0) break;
							// 우선순위 수집
							temp += m_PriorityPos[pos.y - j][pos.x];
							break;
						case RIGHT:
							if ((pos.x + j) >= maxWidth) break;
							temp += m_PriorityPos[pos.y][pos.x + j];
							break;
						case DOWN:
							if ((pos.y + j) >= maxHeight) break;
							temp += m_PriorityPos[pos.y + j][pos.x];
							break;
						case LEFT:
							if ((pos.x - j) < 0) break;
							temp += m_PriorityPos[pos.y][pos.x - j];
							break;
						}
					}
					// 현재 방향의 수집된 우선순위가 높으면 갱신
					if (temp > collectedPriority)
					{
						collectedPriority = temp;
						m_PriorityDir = (Direction)i;
					}
				}
				// 가능성이 가장 높은 방향의 한 칸 앞의 우선순위를 최대치로 설정
				bool loop = ON;
				int limit = 4;
				while (loop)
				{
					if(--limit < 0) break;
					switch (m_PriorityDir)
					{
					case UP:
						// 한 칸 앞이 보드판 밖이면, 우선순위 변동 없이 게임 진행
						if ((pos.y - 1) < 0 || m_PriorityPos[pos.y - 1][pos.x] < 0)
						{
							m_PriorityDir = RIGHT;
							break;
						}
						m_PriorityPos[pos.y - 1][pos.x] = HIGHEST_PRIORITY;
						break;
					case RIGHT:
						if ((pos.x + 1) >= maxWidth || m_PriorityPos[pos.y][pos.x + 1] < 0)
						{
							m_PriorityDir = DOWN;
							break;
						}
						m_PriorityPos[pos.y][pos.x + 1] = HIGHEST_PRIORITY;
						break;
					case DOWN:
						if ((pos.y + 1) >= maxHeight || m_PriorityPos[pos.y + 1][pos.x] < 0)
						{
							m_PriorityDir = LEFT;
							break;
						}
						m_PriorityPos[pos.y + 1][pos.x] = HIGHEST_PRIORITY;
						break;
					case LEFT:
						if ((pos.x - 1) < 0 || m_PriorityPos[pos.y][pos.x - 1] < 0)
						{
							m_PriorityDir = UP;
							break;
						}
						m_PriorityPos[pos.y][pos.x - 1] = HIGHEST_PRIORITY;
						break;
					}
				}
			}
			// 이번이 첫 히트가 아니고 첫 히트와 붙어있다면, 첫 히트의 연장선 상으로 진격한다.
			else
			{
				int nextX = pos.x * 2 - m_FirstHitPos.x;
				int nextY = pos.y * 2 - m_FirstHitPos.y;

				if (!(nextX > pos.x + 1 || nextX < pos.x - 1 ||
					nextY > pos.y + 1 || nextY < pos.y - 1 ||
					nextX < 0 || nextX >= maxWidth ||
					nextY < 0 || nextY >= maxHeight))
					m_PriorityPos[nextY][nextX] = HIGHEST_PRIORITY;
			}
		}
		// 저번 결과가 히트면,
		else
		{
			// 우선 저번 히트의 주변 우선순위를 낮추고
			if (m_PrevHitPos.x != -1)
			{
				for (int i = -1; i < 2; ++i)
				{
					for (int j = -1; j < 2; ++j)
					{
						if ((m_PrevHitPos.y + i) < 0 || (m_PrevHitPos.y + i) >= maxHeight ||
							(m_PrevHitPos.x + j) < 0 || (m_PrevHitPos.x + j) >= maxWidth) break;

						if ((i + j) % 2 != 0)
							m_PriorityPos[m_PrevHitPos.y + i][m_PrevHitPos.x + j] += LITTLE_DECREASE;
						else
							m_PriorityPos[m_PrevHitPos.y + i][m_PrevHitPos.x + j] += LITTLE_DECREASE;
					}
				}
			}
			// 이전과 같은 방향의 한 칸 앞의 우선순위를 최대치로 설정
			switch (m_PriorityDir)
			{
			case UP:
				if ((pos.y - 1) < 0) break;
				m_PriorityPos[pos.y - 1][pos.x] = HIGHEST_PRIORITY;
				break;
			case RIGHT:
				if ((pos.x + 1) >= maxWidth) break;
				m_PriorityPos[pos.y][pos.x + 1] = HIGHEST_PRIORITY;
				break;
			case DOWN:
				if ((pos.y + 1) >= maxHeight) break;
				m_PriorityPos[pos.y + 1][pos.x] = HIGHEST_PRIORITY;
				break;
			case LEFT:
				if ((pos.x - 1) < 0) break;
				m_PriorityPos[pos.y][pos.x - 1] = HIGHEST_PRIORITY;
				break;
			}
		}
		m_PrevHitResult = HIT;
		m_PrevHitPos = pos;
		break;

		// 이번 공격이 미스일때,
	case MISS:
		// 저번 결과가 히트면 첫 히트 좌표의 반대 방향을 우선탐색
		if (m_PrevHitResult == HIT)
		{
			// 방향 뒤집기
			m_PriorityDir = (Direction)((m_PriorityDir + 2) % 4);
			switch (m_PriorityDir)
			{
			case UP:
				if ((m_FirstHitPos.y - 1) < 0) break;
				m_PriorityPos[m_FirstHitPos.y - 1][m_FirstHitPos.x] = HIGHEST_PRIORITY;
				break;
			case RIGHT:
				if ((m_FirstHitPos.x + 1) >= maxWidth) break;
				m_PriorityPos[m_FirstHitPos.y][m_FirstHitPos.x + 1] = HIGHEST_PRIORITY;
				break;
			case DOWN:
				if ((m_FirstHitPos.y + 1) >= maxHeight) break;
				m_PriorityPos[m_FirstHitPos.y + 1][m_FirstHitPos.x] = HIGHEST_PRIORITY;
				break;
			case LEFT:
				if ((m_FirstHitPos.x - 1) < 0) break;
				m_PriorityPos[m_FirstHitPos.y][m_FirstHitPos.x - 1] = HIGHEST_PRIORITY;
				break;
			}
		}
		// 저번 결과가 히트가 아니면,
		else
		{
			// 주변의 우선순위를 약간 내리고
			for (int i = -1; i < 2; ++i)
			{
				for (int j = -1; j < 2; ++j)
				{
					if ((pos.y + i) < 0 || (pos.y + i) >= maxHeight ||
						(pos.x + j) < 0 || (pos.x + j) >= maxWidth) break;

					if ((i + j) % 2 != 0)
						m_PriorityPos[pos.y + i][pos.x + j] += LITTLE_DECREASE;
				}
			}

			// 마지막 히트 좌표 주변의 우선순위를 많이 높인다.
			if (m_PrevHitPos.x != -1)
			{
				for (int i = -1; i < 2; ++i)
				{
					for (int j = -1; j < 2; ++j)
					{
						if ((m_PrevHitPos.y + i) < 0 || (m_PrevHitPos.y + i) >= maxHeight ||
							(m_PrevHitPos.x + j) < 0 || (m_PrevHitPos.x + j) >= maxWidth) break;

						if ((i + j) % 2 != 0)
							m_PriorityPos[m_PrevHitPos.y + i][m_PrevHitPos.x + j] += VERY_HIGH_INCREASE;
					}
				}
			}
		}
		m_PrevHitResult = MISS;
		break;
	case DESTROY_AIRCRAFT:
	case DESTROY_BATTLESHIP:
	case DESTROY_CRUISER:
	case DESTROY_DESTROYER:
		// 배가 침몰하면, 첫 히트 값을 다시 초기화 한다.
		// 또한 주변의 우선순위를 내린다.
		for (int i = -1; i < 2; ++i)
		{
			for (int j = -1; j < 2; ++j)
			{
				if ((pos.y + i) < 0 || (pos.y + i) >= maxHeight ||
					(pos.x + j) < 0 || (pos.x + j) >= maxWidth) break;

				if ((i + j) % 2 != 0)
					m_PriorityPos[pos.y + i][pos.x + j] += LITTLE_DECREASE;
				else
					m_PriorityPos[pos.y + i][pos.x + j] += 0;
			}
		}
		for (int i = -1; i < 2; ++i)
		{
			for (int j = -1; j < 2; ++j)
			{
				if ((m_FirstHitPos.y + i) < 0 || (m_FirstHitPos.y + i) >= maxHeight ||
					(m_FirstHitPos.x + j) < 0 || (m_FirstHitPos.x + j) >= maxWidth) break;

				if ((i + j) % 2 != 0)
					m_PriorityPos[m_FirstHitPos.y + i][m_FirstHitPos.x + j] += 0;
				else
					m_PriorityPos[m_FirstHitPos.y + i][m_FirstHitPos.x + j] += LITTLE_DECREASE;
			}
		}
		m_FirstHitPos = { -1, -1 };
		m_PrevHitResult = hit;
		break;
	}
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

	int highstPriority = LOWEST_PRIORITY;
	int limitPriority = HIGHEST_PRIORITY*10;
	do{
		highstPriority = LOWEST_PRIORITY;
		//가장 높은 우선순위 값 검색
		for (int i = 0; i < maxHeight; ++i)
		{
			for (int j = 0; j < maxWidth; ++j)
			{
				if (m_PriorityPos[i][j] >= limitPriority) continue;
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
						m_PriorityPos[i][j] = LOWEST_PRIORITY;
					}
					else
					{
						//공격 가능한 곳이면, 우선순위 박탈 후 해당 좌표 반환
						m_PriorityPos[i][j] = LOWEST_PRIORITY;
						return{ j, i };
					}
				}
			}
		}
		//위의 반복문이 끝났다는 것은, 해당 우선순위의 좌표 중 공격 가능한 곳이 없다는 것
		//따라서 해당 우선순위보다 낮은 한도에서 우선순위를 다시 검색한다.
		limitPriority = highstPriority;
	} while (highstPriority != LOWEST_PRIORITY);
	//highstPriority가 LOWEST_PRIORITY 라는 것은 공격할 곳이 아무데도 없다는 뜻
	return{ -1, -1 };
}
