#include "stdafx.h"
#include "AI.h"


AI::AI()
{
	m_PlayerType	= AI_PLAYER;
	m_MaxShipSize	= 5;
	m_PrevHitResult = NO_RESULT;
	m_PriorityDir	= DOWN;
	m_FirstHitPos	= { -1, -1 };
	m_PrevHitPos	= { -1, -1 };

	int maxHeight	= m_MyBoard->GetMaxHeight();
	int maxWidth	= m_MyBoard->GetMaxWidth();

	// 2차원 배열 동적 할당
	m_PriorityPos = new int*[maxHeight];
	for (int y = 0; y < maxHeight; ++y)
		m_PriorityPos[y] = new int[maxWidth];
	// 초기화
	for (int y = 0; y < maxHeight; ++y)
	{
		for (int x = 0; x < maxWidth; ++x)
			m_PriorityPos[y][x] = ZERO;
	}
	InitPriority();
}


AI::~AI()
{
	// 동적 할당 해제
	for (int i = 0; i < m_MyBoard->GetMaxHeight(); ++i)
		delete[] m_PriorityPos[i];
	delete[] m_PriorityPos;
}

/*
	AI는 플레이어 초기화 시 추가적으로 우선순위 정보도 초기화 한다.
*/
void AI::InitPlayer()
{
	Player::InitPlayer();
	InitPriority();
}

/*
	우선순위에 관련된 모든 것을 초기화 하는 함수
*/
void AI::InitPriority()
{
	m_MaxShipSize	= 5;
	m_PrevHitResult = NO_RESULT;
	m_PriorityDir	= DOWN;
	m_FirstHitPos	= { -1, -1 };
	m_PrevHitPos	= { -1, -1 };

	int maxHeight	= m_MyBoard->GetMaxHeight();
	int maxWidth	= m_MyBoard->GetMaxWidth();

	// 우선순위 테이블 초기화
	for (int y = 0; y < maxHeight; ++y)
	{
		for (int x = 0; x < maxWidth; ++x)
		{
			// 모든 배는 연속된 타일에 놓여있으므로, 기본적으로 한 종류 타일에만 우선순위를 좀 더 부여
			if ((x + y) % 2 == 0)
			{
				m_PriorityPos[y][x] = NORMAL_INCREASE;
				// 그 중에서도 효율적으로 맵을 훑을 수 있도록 추가 우선순위를 부여
				if ((x + y) % 4 == 2)
					m_PriorityPos[y][x] += LITTLE_INCREASE;
			}
			else
				m_PriorityPos[y][x] = ZERO;
		}
	}
}

/*
	공격한 좌표(pos)와 공격결과(hit)를 받아와 우선순위를 갱신하는 함수
*/
void AI::UpdatePriority(Position pos, HitResult hit)
{
	switch (hit)
	{
		// 이번 공격이 히트일 때,
	case HIT:
		// 먼저 현재 히트 좌표 상하좌우의 우선순위를 높힌다.
		RangeUpdate(1, pos, HIGH_INCREASE, LITTLE_DECREASE);

		// 저번 결과가 히트가 아니면 두 가지 경우의 수를 생각한다.
		if (m_PrevHitResult != HIT)
		{
			// 이번이 첫 히트이면, 주변 네 방향 중 가능성이 높은 곳을 계산하고 그쪽으로 진격한다.
			if (m_FirstHitPos.x == -1)
			{
				m_FirstHitPos = pos;
				
				// 가능성이 가장 높은 방향 계산
				m_PriorityDir = DecideBestDir(pos);

				// 가능성이 가장 높은 방향의 한 칸 앞의 우선순위를 최대치로 설정
				DecideNextAttack(pos, m_PriorityDir);
			}
			// 이번이 첫 히트가 아니고 첫 히트와 붙어있다면, 첫 히트의 연장선 상으로 진격한다.
			else
			{
				int nextX = pos.x * 2 - m_FirstHitPos.x;
				int nextY = pos.y * 2 - m_FirstHitPos.y;

				if (m_MyBoard->IsInBoard(nextX, nextY) &&
					m_PriorityPos[nextY][nextX] != LOWEST_PRIORITY &&
					!(nextX > pos.x + 1 || nextX < pos.x - 1 ||
					nextY > pos.y + 1 || nextY < pos.y - 1))
					m_PriorityPos[nextY][nextX] = HIGHEST_PRIORITY;
			}
		}
		// 저번 결과가 히트면,
		else
		{
			// 우선 저번 히트의 주변 우선순위를 낮추고
			RangeUpdate(1, m_PrevHitPos, LITTLE_DECREASE, NORMAL_DECREASE);

			// 이전과 같은 방향의 한 칸 앞의 우선순위를 최대치로 설정
			DecideNextAttack(pos, m_PriorityDir);
		}
		m_PrevHitResult = HIT;
		m_PrevHitPos = pos;
		break;

		// 이번 공격이 미스일 때,
	case MISS:
		// 저번 결과가 히트면,
		if (m_PrevHitResult == HIT)
		{
			// 방향 뒤집기
			m_PriorityDir = (Direction)((m_PriorityDir + 2) % 4);

			// 첫 히트 좌표의 반대 방향을 우선탐색
			DecideNextAttack(m_FirstHitPos, m_PriorityDir);
		}
		// 저번 결과가 히트가 아니면,
		else
		{
			// 주변의 우선순위를 약간 내리고
			RangeUpdate(1, pos, LITTLE_DECREASE, ZERO);

			// 마지막 히트 좌표 주변의 우선순위를 많이 높인다.
			RangeUpdate(1, m_PrevHitPos, VERY_HIGH_INCREASE, ZERO);
		}
		m_PrevHitResult = MISS;
		break;
		// 이번 공격에 배가 침몰 했을 때,
	case DESTROY_AIRCRAFT:
	case DESTROY_BATTLESHIP:
	case DESTROY_CRUISER:
	case DESTROY_DESTROYER:
		// 먼저 주변의 우선순위를 내린다.
		RangeUpdate(1, pos, LITTLE_DECREASE, ZERO);
		RangeUpdate(1, m_FirstHitPos, LITTLE_INCREASE, LITTLE_DECREASE);
		// 그리고 첫 히트 값을 다시 초기화 한다.
		m_FirstHitPos = { -1, -1 };
		m_PrevHitResult = hit;
		break;
	}
}

/*
	배를 배치하는 함수
*/
void AI::SettingShips()
{
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();
	Position startPos = { 0, 0 };

	for (auto& ship : m_ShipList)
	{
		int maxHp = ship->GetMaxHP();
		Direction direction = Direction(UP);

		do {
			direction = (Direction)(rand() % 4);
			startPos.x = rand() % maxWidth;
			startPos.y = rand() % maxHeight;
		} while (!IsValidShipPosition(startPos, direction, maxHp));

		PlaceShip(ship, startPos, direction);
	}
}

/*
	우선순위가 가장 높을 곳을 찾아 공격하는 함수
*/
Position AI::Attack()
{
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();
	int highstPriority = LOWEST_PRIORITY;

	do{
		highstPriority = LOWEST_PRIORITY;
		
		//가장 높은 우선순위 값 검색
		for (int y = 0; y < maxHeight; ++y)
		{
			for (int x = 0; x < maxWidth; ++x)
			{
				if (m_PriorityPos[y][x] > highstPriority)
					highstPriority = m_PriorityPos[y][x];
			}
		}
		//검색된 우선순위 값에 해당하는 좌표 검색
		for (int y = 0; y < maxHeight; ++y)
		{
			for (int x = 0; x < maxWidth; ++x)
			{
				if (m_PriorityPos[y][x] == highstPriority)
				{
					//검색된 좌표가 공격 가능한 곳인지 확인
					if (!m_EnemyBoard->IsValidAttack(x, y))
					{
						//공격 불가능한 곳이면, 우선순위만 박탈
						m_PriorityPos[y][x] = LOWEST_PRIORITY;
					}
					else
					{
						//주변이 갇혀있다면 우선순위를 박탈하고 건너뛰기
						PriorityCalcRate temp = m_EnemyBoard->IsConfinedPos(x, y);
						if (temp != ZERO)
							m_PriorityPos[y][x] += temp;
						else
						{
							//공격 가능한 곳이면, 우선순위 박탈 후 해당 좌표 반환
							m_PriorityPos[y][x] = LOWEST_PRIORITY;
							return{ x, y };
						}
					}
				}
			}
		}
		//위의 반복문이 끝났다는 것은, 해당 우선순위의 좌표 중 공격 가능한 곳이 없다는 것
		//따라서 우선순위를 다시 검색한다.
	} while (highstPriority != LOWEST_PRIORITY);
	//highstPriority가 LOWEST_PRIORITY 라는 것은 공격할 곳이 아무데도 없다는 뜻
	_ASSERT(highstPriority != LOWEST_PRIORITY);
	return{ -1, -1 };
}

/*
	범위의 크기(range)와 중앙좌표(pos)와 상하좌우에 적용할 수치(crossForm)와 대각선에 적용할 수치(xForm)를 받아
	우선순위 값을 변경하는 함수
	- 참고로 range가 2 이상이면, 체크무늬 형태로 적용된다.
*/
void AI::RangeUpdate(int range, Position pos, PriorityCalcRate crossForm, PriorityCalcRate xForm)
{
	int dx = 0;
	int dy = 0;
	for (dy = -range; dy < range * 2; ++dy)
	{
		for (dx = -range; dx < range * 2; ++dx)
		{
			int x, y;
			x = pos.x + dx;
			y = pos.y + dy;
			if (!m_MyBoard->IsInBoard(x, y))
				continue;
			if (m_PriorityPos[y][x] == LOWEST_PRIORITY)
				continue;

			// 상하좌우
			if ((dx + dy) % 2 != 0)
				m_PriorityPos[y][x] += crossForm;
			// 대각선
			else
				m_PriorityPos[y][x] += xForm;
		}
	}
}

/*
	현재 좌표(pos)와 나아갈 방향(dir)을 받아 한 칸 앞의 좌표의 우선순위를 최대치로 만드는 함수
*/
void AI::DecideNextAttack(Position pos, Direction dir)
{
	// 방향에 따른 좌표 증감 변수 설정
	int dx = 0;
	int dy = 0;
	switch (dir)
	{
	case UP:	dx = 0;	dy = -1;	break;
	case RIGHT:	dx = 1;	dy = 0;	break;
	case DOWN:	dx = 0;	dy = 1;	break;
	case LEFT:	dx = -1;	dy = 0;	break;
	}

	// 해당 방향의 한 칸 앞을 최고 우선순위로 설정
	int x, y;
	x = pos.x + dx;
	y = pos.y + dy;
	if (!m_MyBoard->IsInBoard(x, y))
		return;
	if (m_PriorityPos[y][x] == LOWEST_PRIORITY)
		return;
	m_PriorityPos[y][x] = HIGHEST_PRIORITY;
}

/*
	현재 좌표(pos)를 받아 가장 확률이 좋은 방향을 계산해 반환하는 함수
*/
Direction AI::DecideBestDir(Position pos)
{
	Direction BestDir = DOWN;
	int HighstPriority = 0;
	int temp;
	int dx = 0;
	int dy = 0;

	// 4가지 방향을 각각 순회
	for (int dir = UP; dir <= LEFT; ++dir)
	{
		temp = 0;
		// 방향에 따른 증감 변수 설정
		switch (dir)
		{
		case UP:	dx = 0;	dy = -1;	break;
		case RIGHT:	dx = 1;	dy = 0;	break;
		case DOWN:	dx = 0;	dy = 1;	break;
		case LEFT:	dx = -1;	dy = 0;	break;
		}

		// 원점에서부터 설정된 방향을 향해 한 칸씩 이동하며 temp에 우선순위 저장
		for (int n = 1; n <= m_MaxShipSize; ++n)
		{
			int x, y;
			x = pos.x + dx*n;
			y = pos.y + dy*n;
			if (!m_MyBoard->IsInBoard(x, y))
				break;
			temp += m_PriorityPos[y][x];
		}

		// 최고 우선순위와, 해당 방향을 갱신
		if (temp > HighstPriority)
		{
			HighstPriority = temp;
			BestDir = (Direction)dir;
		}
	}
	return BestDir;
}
