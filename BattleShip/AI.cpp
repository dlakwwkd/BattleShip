#include "stdafx.h"
#include "AI.h"


AI::AI()
{
	m_PlayerType = AI_PLAYER;
	m_MaxShipSize = 5;
	m_PrevHitResult = NO_RESULT;
	m_PriorityDir = DOWN;
	m_FirstHitPos = { -1, -1 };
	m_PrevHitPos = { -1, -1 };

	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();

	m_PriorityPos = new int*[maxHeight];
	for (int y = 0; y < maxHeight; ++y)
	{
		m_PriorityPos[y] = new int[maxWidth];
	}
	for (int y = 0; y < maxHeight; ++y)
	{
		for (int x = 0; x < maxWidth; ++x)
		{
			m_PriorityPos[y][x] = 0;
		}
	}
	InitPriority();
}


AI::~AI()
{
	for (int i = 0; i < m_MyBoard->GetMaxHeight(); ++i)
	{
		delete[] m_PriorityPos[i];
	}
	delete[] m_PriorityPos;
}

void AI::InitPlayer()
{
	Player::InitPlayer();
	InitPriority();
}

void AI::InitPriority()
{
	m_MaxShipSize = 5;
	m_PrevHitResult = NO_RESULT;
	m_PriorityDir = DOWN;
	m_FirstHitPos = { -1, -1 };
	m_PrevHitPos = { -1, -1 };

	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();

	// �켱���� ���̺� �ʱ�ȭ
	for (int y = 0; y < maxHeight; ++y)
	{
		for (int x = 0; x < maxWidth; ++x)
		{
			// ��� ��� ���ӵ� Ÿ�Ͽ� ���������Ƿ�, �⺻������ �� ���� Ÿ�Ͽ��� �켱������ �� �� �ο�
			if ((x + y) % 2 == 0)
			{
				m_PriorityPos[y][x] = NORMAL_INCREASE;
				// �� �߿����� ȿ�������� ���� ���� �� �ֵ��� �߰� �켱������ �ο�
				if ((x + y) % 4 == 2)
					m_PriorityPos[y][x] += LITTLE_INCREASE;
			}
			else
				m_PriorityPos[y][x] = 0;

			
		}
	}
}

void AI::UpdatePriority(Position pos, HitResult hit)
{
	switch (hit)
	{
		// �̹� ������ ��Ʈ�� ��,
	case HIT:
		// ���� ���� ��Ʈ ��ǥ �����¿��� �켱������ ������.
		RangeUpdate(1, pos, HIGH_INCREASE, LITTLE_DECREASE);

		// ���� ����� ��Ʈ�� �ƴϸ� �� ���� ����� ���� �����Ѵ�.
		if (m_PrevHitResult != HIT)
		{
			// �̹��� ù ��Ʈ�̸�, �ֺ� �� ���� �� ���ɼ��� ���� ���� ����ϰ� �������� �����Ѵ�.
			if (m_FirstHitPos.x == -1)
			{
				m_FirstHitPos = pos;
				
				// ���ɼ��� ���� ���� ���� ���
				m_PriorityDir = DecideBestDir(pos);

				// ���ɼ��� ���� ���� ������ �� ĭ ���� �켱������ �ִ�ġ�� ����
				DecideNextAttack(pos, m_PriorityDir);
			}
			// �̹��� ù ��Ʈ�� �ƴϰ� ù ��Ʈ�� �پ��ִٸ�, ù ��Ʈ�� ���弱 ������ �����Ѵ�.
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
		// ���� ����� ��Ʈ��,
		else
		{
			// �켱 ���� ��Ʈ�� �ֺ� �켱������ ���߰�
			RangeUpdate(1, m_PrevHitPos, LITTLE_DECREASE, NORMAL_DECREASE);

			// ������ ���� ������ �� ĭ ���� �켱������ �ִ�ġ�� ����
			DecideNextAttack(pos, m_PriorityDir);
		}
		m_PrevHitResult = HIT;
		m_PrevHitPos = pos;
		break;

		// �̹� ������ �̽��� ��,
	case MISS:
		// ���� ����� ��Ʈ��,
		if (m_PrevHitResult == HIT)
		{
			// ���� ������
			m_PriorityDir = (Direction)((m_PriorityDir + 2) % 4);

			// ù ��Ʈ ��ǥ�� �ݴ� ������ �켱Ž��
			DecideNextAttack(m_FirstHitPos, m_PriorityDir);
		}
		// ���� ����� ��Ʈ�� �ƴϸ�,
		else
		{
			// �ֺ��� �켱������ �ణ ������
			RangeUpdate(1, pos, LITTLE_DECREASE, ZERO);

			// ������ ��Ʈ ��ǥ �ֺ��� �켱������ ���� ���δ�.
			RangeUpdate(1, m_PrevHitPos, VERY_HIGH_INCREASE, ZERO);
		}
		m_PrevHitResult = MISS;
		break;
		// �̹� ���ݿ� �谡 ħ�� ���� ��,
	case DESTROY_AIRCRAFT:
	case DESTROY_BATTLESHIP:
	case DESTROY_CRUISER:
	case DESTROY_DESTROYER:
		// ���� �ֺ��� �켱������ ������.
		RangeUpdate(1, pos, LITTLE_DECREASE, ZERO);
		RangeUpdate(1, m_FirstHitPos, LITTLE_INCREASE, LITTLE_DECREASE);
		// �׸��� ù ��Ʈ ���� �ٽ� �ʱ�ȭ �Ѵ�.
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

	do{
		highstPriority = LOWEST_PRIORITY;
		
		//���� ���� �켱���� �� �˻�
		for (int y = 0; y < maxHeight; ++y)
		{
			for (int x = 0; x < maxWidth; ++x)
			{
				if (m_PriorityPos[y][x] > highstPriority)
					highstPriority = m_PriorityPos[y][x];
			}
		}
		//�˻��� �켱���� ���� �ش��ϴ� ��ǥ �˻�
		for (int y = 0; y < maxHeight; ++y)
		{
			for (int x = 0; x < maxWidth; ++x)
			{
				if (m_PriorityPos[y][x] == highstPriority)
				{
					//�˻��� ��ǥ�� ���� ������ ������ Ȯ��
					if (!m_EnemyBoard->IsValidAttack(x, y))
					{
						//���� �Ұ����� ���̸�, �켱������ ��Ż
						m_PriorityPos[y][x] = LOWEST_PRIORITY;
					}
					else
					{
						//�ֺ��� �����ִٸ� �켱������ ��Ż�ϰ� �ǳʶٱ�
						PriorityCalcRate temp = m_EnemyBoard->IsConfinedPos(x, y);
						if (temp != ZERO)
							m_PriorityPos[y][x] += temp;
						else
						{
							//���� ������ ���̸�, �켱���� ��Ż �� �ش� ��ǥ ��ȯ
							m_PriorityPos[y][x] = LOWEST_PRIORITY;
							return{ x, y };
						}
					}
				}
			}
		}
		//���� �ݺ����� �����ٴ� ����, �ش� �켱������ ��ǥ �� ���� ������ ���� ���ٴ� ��
		//���� �켱������ �ٽ� �˻��Ѵ�.
	} while (highstPriority != LOWEST_PRIORITY);
	//highstPriority�� LOWEST_PRIORITY ��� ���� ������ ���� �ƹ����� ���ٴ� ��

	return{ -1, -1 };
}

void AI::RangeUpdate(int range, Position& pos, PriorityCalcRate crossForm, PriorityCalcRate xForm)
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

			// �����¿�
			if ((dx + dy) % 2 != 0)
				m_PriorityPos[y][x] += crossForm;
			// �밢��
			else
				m_PriorityPos[y][x] += xForm;
		}
	}
}

void AI::DecideNextAttack(Position& pos, Direction dir)
{
	// ���⿡ ���� ��ǥ ���� ���� ����
	int dx = 0;
	int dy = 0;
	switch (dir)
	{
	case UP:	dx = 0;	dy = -1;	break;
	case RIGHT:	dx = 1;	dy = 0;	break;
	case DOWN:	dx = 0;	dy = 1;	break;
	case LEFT:	dx = -1;	dy = 0;	break;
	}

	// �ش� ������ �� ĭ ���� �ְ� �켱������ ����
	int x, y;
	x = pos.x + dx;
	y = pos.y + dy;
	if (!m_MyBoard->IsInBoard(x, y))
		return;
	if (m_PriorityPos[y][x] == LOWEST_PRIORITY)
		return;
	m_PriorityPos[y][x] = HIGHEST_PRIORITY;
}

Direction AI::DecideBestDir(Position& pos)
{
	Direction BestDir = DOWN;
	int HighstPriority = 0;
	int temp;
	int dx = 0;
	int dy = 0;

	// 4���� ������ ���� ��ȸ
	for (int dir = UP; dir <= LEFT; ++dir)
	{
		temp = 0;
		// ���⿡ ���� ���� ���� ����
		switch (dir)
		{
		case UP:	dx = 0;	dy = -1;	break;
		case RIGHT:	dx = 1;	dy = 0;	break;
		case DOWN:	dx = 0;	dy = 1;	break;
		case LEFT:	dx = -1;	dy = 0;	break;
		}

		// ������������ ������ ������ ���� �� ĭ�� �̵��ϸ� temp�� �켱���� ����
		for (int n = 1; n <= m_MaxShipSize; ++n)
		{
			int x, y;
			x = pos.x + dx*n;
			y = pos.y + dy*n;
			if (!m_MyBoard->IsInBoard(x, y))
				break;
			temp += m_PriorityPos[y][x];
		}

		// �ְ� �켱������, �ش� ������ ����
		if (temp > HighstPriority)
		{
			HighstPriority = temp;
			BestDir = (Direction)dir;
		}
	}
	return BestDir;
}
