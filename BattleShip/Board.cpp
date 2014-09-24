#include "stdafx.h"
#include "Board.h"
#include "Print.h"


Board::Board()
{
	m_Name		= "UnNamed Board";
	m_Height	= 8;
	m_Width		= 8;

	// 2���� �迭 ���� �Ҵ�
	m_Board = new int*[m_Height];
	for (int dy = 0; dy < m_Height; ++dy)
		m_Board[dy] = new int[m_Width];
	// �ʱ�ȭ
	for (int dy = 0; dy < m_Height; ++dy)
	{
		for (int dx = 0; dx < m_Width; ++dx)
			m_Board[dy][dx] = NO_RESULT;
	}
}


Board::~Board()
{
	// ���� �Ҵ� ����
	for (int dy = 0; dy < m_Height; ++dy)
		delete[] m_Board[dy];
	delete[] m_Board;
}

/*
	���� ���¸� �ʱ�ȭ �ϴ� �Լ�
*/
void Board::InitBoard()
{
	for (int dy = 0; dy < m_Height; ++dy)
	{
		for (int dx = 0; dx < m_Width; ++dx)
			m_Board[dy][dx] = NO_RESULT;
	}
}

/*
	������ ��ġ(pos)�� �޾Ƽ� ScreenBuffer�� �׸��� �Լ�
*/
void Board::PrintBoard(Position pos)
{
	_ASSERT(!(pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES));
	if (pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;

	// ���� �̸� ����
	Print::Instance().InText(pos.x - 1, pos.y - 2, m_Name);
	// ���ڹ��� �׸���
	Print::Instance().InColor(pos.x - 2, pos.y + m_Height, Color::MARK_BOX);
	Print::Instance().InColor(pos.x - 2, pos.y - 1, Color::MARK_BOX);
	Print::Instance().InColor(pos.x + m_Width*2, pos.y + m_Height, Color::MARK_BOX);
	Print::Instance().InColor(pos.x + m_Width*2, pos.y - 1, Color::MARK_BOX);

	int dx, dy;
	// ������ �ٱ� Ʋ �׸���
	for (dx = -2; dx < m_Width + 1; ++dx)	Print::Instance().InColor(pos.x + dx*2, pos.y - 2, Color::A_DEATH);
	for (dy = -2; dy < m_Height + 1; ++dy)	Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::B_DEATH);
	for (; dx > -2; --dx)					Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::C_DEATH);
	for (; dy > -2; --dy)					Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::D_DEATH);
	// ���� Ʋ �׸���
	for (dx = 0; dx < m_Width; ++dx)		Print::Instance().InColor(pos.x + dx*2, pos.y - 1, Color::BOX);
	for (dy = 0; dy < m_Height; ++dy)		Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::BOX);

	// x ��ǥ�� �׸���
	for (int dx = 0; dx < m_Width; ++dx)
	{
		Print::Instance().InText(pos.x + dx*2 + 1, pos.y + m_Height, CoordTrans(CHAR_X1 + (char)dx));
		Print::Instance().InColor(pos.x + dx * 2, pos.y + m_Height, Color::COORD_BOX);
	}
	// y ��ǥ�� �׸���
	for (int dy = 0; dy < m_Height; ++dy)
	{
		Print::Instance().InText(pos.x - 1, pos.y + dy, CoordTrans(CHAR_Y1 + (char)dy));
		Print::Instance().InColor(pos.x - 2, pos.y + dy, Color::COORD_BOX);
	}

	// ���� ���� üũ���� �׸���
	for (int dy = 0; dy < m_Height; ++dy)
	{
		for (int dx = 0; dx < m_Width; ++dx)
		{
			if ((dx + dy) % 2 == 1)
				Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::TILE_1);
			else
				Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::TILE_2);
		}
	}
}

/*
	������ ��ġ(pos)�� ����(owner)�� �޾Ƽ� ������ ���� ���¸� ScreenBuffer�� �����ϴ� �Լ�
	- owner�� �ڽ�(IAM)���� ��(ENEMY)������ ���� ��Ʈ ó���� �ٸ���.
*/
void Board::UpdateBoard(Position pos, Owner owner)
{
	_ASSERT(!(pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES));
	if (pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;

	Color color = DEF;
	for (int dy = 0; dy < m_Height; ++dy)
	{
		for (int dx = 0; dx < m_Width; ++dx)
		{
			int shipPart = m_Board[dy][dx];

			// shipPart�� ����� ��Ʈ���� ���� ���� �Ϻκ����� �ǹ�
			if (shipPart > 0)
			{
				// �ڽ��� ���忡�� ����ִ� �谡 ǥ�õ�
				switch (owner)
				{
				case IAM:
					switch (shipPart)
					{
					case AIRCRAFT:	color = A_LIVE;	break;
					case BATTLESHIP:color = B_LIVE;	break;
					case CRUISER:	color = C_LIVE;	break;
					case DESTROYER:	color = D_LIVE;	break;
					}
					Print::Instance().InColor(pos.x + dx * 2, pos.y + dy, color);
					break;
				case ENEMY:
					break;
				}
			}
			// shipPart�� ������ ������ ������ ��ǥ���� �ǹ�
			else if (shipPart < 0)
			{
				// ������ ���ο� ���� ��Ʈ�� ���� �κ��� ǥ���ϴ� ����� �޶���
				switch (owner)
				{
				case IAM:	// �ڽ��� ���忡�� ��Ʈ�� ��� ��� �� ���� �� ǥ��
					switch (shipPart)
					{
					case -AIRCRAFT:			
					case DESTROY_AIRCRAFT:	color = A_DEATH; break;

					case -BATTLESHIP:
					case DESTROY_BATTLESHIP:color = B_DEATH; break;

					case -CRUISER:
					case DESTROY_CRUISER:	color = C_DEATH; break;

					case -DESTROYER:
					case DESTROY_DESTROYER:	color = D_DEATH; break;

						// �̽��� ���� Ÿ�� ���� �״�� ����
					case MISS:
						if ((dy + dx) % 2 == 1)
							color = TILE_1;
						else
							color = TILE_2;
						break;
					}
					break;
				case ENEMY:	// ���� ���忡�� �ı��� �踸 �� ���� �� ǥ��
					switch (shipPart)
					{
					case DESTROY_AIRCRAFT:	color = A_DEATH; break;
					case DESTROY_BATTLESHIP:color = B_DEATH; break;
					case DESTROY_CRUISER:	color = C_DEATH; break;
					case DESTROY_DESTROYER:	color = D_DEATH; break;
					case MISS:
						if ((dy + dx) % 2 == 1)
							color = TILE_1;
						else
							color = TILE_2;
						break;
						// �ı����� �ʰ�, ��Ʈ�� �κ��� ��Ʈ �� ǥ��
					default:
						color = HIT_POS;
						break;
					}
					break;
				}
				Print::Instance().InText(pos.x + dx * 2, pos.y + dy, "[]");
				Print::Instance().InColor(pos.x + dx * 2, pos.y + dy, color);
			}
			// shipPart�� 0�̸� ������ �������� ���� ��ǥ���� �ǹ�
			else{}
		}
	}
}

/*
	���ݹ��� ��ǥ(pos)�� �޾Ƽ� ������ ���¸� �����ϴ� �Լ�
*/
void Board::ProcessAttack(Position pos)
{
	int x = pos.x;
	int y = pos.y;

	_ASSERT(IsInBoard(x, y));
	if (!IsInBoard(x, y))
		return;

	// �谡 ���� ���� ��� MISS ó��
	if (m_Board[y][x] == NO_RESULT)
		m_Board[y][x] = MISS;
	// �谡 �ִ� ���� ��� HIT ó��(� �踦 ��Ÿ���� ���� -1�� ���ϸ� �� ���� ��Ʈ���� ��Ÿ��)
	else
		m_Board[y][x] = -(m_Board[y][x]);
}

/*
	���ݹ��� ��ǥ(pos)�� ���ݰ��(info)�� �޾Ƽ� ������ ���¸� �����ϴ� �Լ�
	- ��Ʈ��ũ ������ ��� ����� ������ �� ��ġ��Ȳ�� �� �� �����Ƿ� �� �Լ��� �̿��Ѵ�.
*/
void Board::ProcessAttack(Position pos, HitResult info)
{
	_ASSERT(IsInBoard(pos.x, pos.y));
	if (!IsInBoard(pos.x, pos.y))
		return;

	m_Board[pos.y][pos.x] = info;
}

/*
	�ı��� ���� ��ǥ����Ʈ(shipPos)�� �޾Ƽ� ������ ���¸� �����ϴ� �Լ�
*/
void Board::ProcessDestroy(std::vector<Position> shipPos)
{
	// ��Ʈó���� ���� -10�� �ϸ� �ı��� ó���� �ȴ�.
	for (auto& pos : shipPos)
	{
		_ASSERT(IsInBoard(pos.x, pos.y));
		if (!IsInBoard(pos.x, pos.y))
			break;

		m_Board[pos.y][pos.x] -= 10;
	}
}

/*
	��ġ�� ��ǥ(pos)�� ���� ����(shipType)�� �޾Ƽ� ������ ���¸� �����ϴ� �Լ�
*/
void Board::AddPosition(Position pos, ShipType shipType)
{
	_ASSERT(IsInBoard(pos.x, pos.y));
	if (!IsInBoard(pos.x, pos.y))
		return;

	m_Board[pos.y][pos.x] = shipType;
}

/*
	�ش� ��ǥ(x,y)�� �谡 ������ true, ������ false�� ����
*/
bool Board::IsShipHere(int x, int y)
{
	if (!IsInBoard(x, y))
		return false;

	if (m_Board[y][x] == 0)
		return false;
	else
		return true;
}

/*
	�ش� ��ǥ(x,y)�� ���� ������ ���̸� true, �ƴϸ� false�� ����
*/
bool Board::IsValidAttack(int x, int y)
{
	if (!IsInBoard(x, y))
		return false;

	if (m_Board[y][x] < 0)
		return false;
	else
		return true;
}

/*
	�ش� ��ǥ(x,y)�� ���忡 �����ϸ� true, �ƴϸ� false�� ����
*/
bool Board::IsInBoard(int x, int y)
{
	if (x < 0 || x >= m_Width ||
		y < 0 || y >= m_Height)
		return false;
	else
		return true;
}

/*
	�ش� ��ǥ(x,y)�� ���������� LOWEST_PRIORITY, �ƴϸ� ZERO�� ����
	- ���߿� �ٸ� ���� ��ȯ���� �߰��� ���ɼ��� �־ ���ϰ��� bool�� ���� ����
*/
PriorityCalcRate Board::IsConfinedPos(int x, int y)
{
	int outsideNum = 0;
	int missNum = 0;
	int destroyNum = 0;

	for (int dy = -1; dy < 2; ++dy)
	{
		for (int dx = -1; dx < 2; ++dx)
		{
			if ((dx + dy) % 2 != 0)
			{
				if (!IsInBoard(x + dx, y + dy))
				{
					outsideNum++;
					break;
				}
				if (m_Board[y + dy][x + dx] == MISS)
					missNum++;
				if (m_Board[y + dy][x + dx] < HIT)
					destroyNum++;
			}
		}
	}
	// Ȯ���ϰ� �谡 ������ ���ɼ��� ���� ���
	if (outsideNum + missNum + destroyNum > 3)
		return LOWEST_PRIORITY;
	else
		return ZERO;
}

/*
	��ǥ��(coord)�� �޾Ƽ� ���ڿ��� ��ȯ�Ͽ� ��ȯ�ϴ� �Լ�
*/
std::string Board::CoordTrans(char coord)
{
	char temp[2];
	temp[0] = coord;
	temp[1] = '\0';
	return temp;
}
