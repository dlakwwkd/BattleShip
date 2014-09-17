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
			// ��� ��� ���ӵ� Ÿ�Ͽ� ���������Ƿ�, �⺻������ �� ���� Ÿ�Ͽ��� �켱������ �� �� �ο�
			if ((i + j) % 2 == 0)
				m_PriorityPos[i][j] = NORMAL_INCREASE;
			else
				m_PriorityPos[i][j] = 0;

			// �� �߿����� ȿ�������� ���� ���� �� ���� ������ �߰� �켱������ �ο�
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
		// �̹� ������ ��Ʈ�϶�,
	case HIT:
		// ���� ���� ��Ʈ ��ǥ �ֺ��� �켱������ ������.
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
		// ���� ����� ��Ʈ�� �ƴϸ� �� ���� ����� ���� �����Ѵ�.
		if (m_PrevHitResult != HIT)
		{
			// �̹��� ù ��Ʈ�̸�, �ֺ� �� ���� �� ���ɼ��� ���� ���� ����ϰ� �������� �����Ѵ�.
			if (m_FirstHitPos.x == -1)
			{
				m_FirstHitPos = pos;

				int collectedPriority = 0;
				int temp = 0;
				// 4���� ���⿡ ���� ������ �� ���̸�ŭ�� �켱������ ����
				for (int i = UP; i <= LEFT; ++i)
				{
					temp = 0;
					for (int j = 0; j < m_PossibleShipLength; ++j)
					{
						switch (i)
						{
						case UP:
							// �������� ����� �켱���� ����;
							if ((pos.y - j) < 0) break;
							// �켱���� ����
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
					// ���� ������ ������ �켱������ ������ ����
					if (temp > collectedPriority)
					{
						collectedPriority = temp;
						m_PriorityDir = (Direction)i;
					}
				}
				// ���ɼ��� ���� ���� ������ �� ĭ ���� �켱������ �ִ�ġ�� ����
				bool loop = ON;
				int limit = 4;
				while (loop)
				{
					if(--limit < 0) break;
					switch (m_PriorityDir)
					{
					case UP:
						// �� ĭ ���� ������ ���̸�, �켱���� ���� ���� ���� ����
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
			// �̹��� ù ��Ʈ�� �ƴϰ� ù ��Ʈ�� �پ��ִٸ�, ù ��Ʈ�� ���弱 ������ �����Ѵ�.
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
		// ���� ����� ��Ʈ��,
		else
		{
			// �켱 ���� ��Ʈ�� �ֺ� �켱������ ���߰�
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
			// ������ ���� ������ �� ĭ ���� �켱������ �ִ�ġ�� ����
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

		// �̹� ������ �̽��϶�,
	case MISS:
		// ���� ����� ��Ʈ�� ù ��Ʈ ��ǥ�� �ݴ� ������ �켱Ž��
		if (m_PrevHitResult == HIT)
		{
			// ���� ������
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
		// ���� ����� ��Ʈ�� �ƴϸ�,
		else
		{
			// �ֺ��� �켱������ �ణ ������
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

			// ������ ��Ʈ ��ǥ �ֺ��� �켱������ ���� ���δ�.
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
		// �谡 ħ���ϸ�, ù ��Ʈ ���� �ٽ� �ʱ�ȭ �Ѵ�.
		// ���� �ֺ��� �켱������ ������.
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
		//���� ���� �켱���� �� �˻�
		for (int i = 0; i < maxHeight; ++i)
		{
			for (int j = 0; j < maxWidth; ++j)
			{
				if (m_PriorityPos[i][j] >= limitPriority) continue;
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
						m_PriorityPos[i][j] = LOWEST_PRIORITY;
					}
					else
					{
						//���� ������ ���̸�, �켱���� ��Ż �� �ش� ��ǥ ��ȯ
						m_PriorityPos[i][j] = LOWEST_PRIORITY;
						return{ j, i };
					}
				}
			}
		}
		//���� �ݺ����� �����ٴ� ����, �ش� �켱������ ��ǥ �� ���� ������ ���� ���ٴ� ��
		//���� �ش� �켱�������� ���� �ѵ����� �켱������ �ٽ� �˻��Ѵ�.
		limitPriority = highstPriority;
	} while (highstPriority != LOWEST_PRIORITY);
	//highstPriority�� LOWEST_PRIORITY ��� ���� ������ ���� �ƹ����� ���ٴ� ��
	return{ -1, -1 };
}
