#include "stdafx.h"
#include "Board.h"
#include "Print.h"


Board::Board()
{
	m_Name		= "UnNamed Board";
	m_Height	= 8;
	m_Width		= 8;

	// 2차원 배열 동적 할당
	m_Board = new int*[m_Height];
	for (int dy = 0; dy < m_Height; ++dy)
		m_Board[dy] = new int[m_Width];
	// 초기화
	for (int dy = 0; dy < m_Height; ++dy)
	{
		for (int dx = 0; dx < m_Width; ++dx)
			m_Board[dy][dx] = NO_RESULT;
	}
}


Board::~Board()
{
	// 동적 할당 해제
	for (int dy = 0; dy < m_Height; ++dy)
		delete[] m_Board[dy];
	delete[] m_Board;
}

/*
	보드 상태를 초기화 하는 함수
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
	보드의 위치(pos)를 받아서 ScreenBuffer에 그리는 함수
*/
void Board::PrintBoard(Position pos)
{
	if (pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;

	// 보드 이름 쓰기
	Print::Instance().InText(pos.x - 1, pos.y - 2, m_Name);
	// 데코무늬 그리기
	Print::Instance().InColor(pos.x - 2, pos.y + m_Height, Color::MARK_BOX);
	Print::Instance().InColor(pos.x - 2, pos.y - 1, Color::MARK_BOX);
	Print::Instance().InColor(pos.x + m_Width*2, pos.y + m_Height, Color::MARK_BOX);
	Print::Instance().InColor(pos.x + m_Width*2, pos.y - 1, Color::MARK_BOX);

	int dx, dy;
	// 보드의 바깥 틀 그리기
	for (dx = -2; dx < m_Width + 1; ++dx)	Print::Instance().InColor(pos.x + dx*2, pos.y - 2, Color::A_DEATH);
	for (dy = -2; dy < m_Height + 1; ++dy)	Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::B_DEATH);
	for (; dx > -2; --dx)					Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::C_DEATH);
	for (; dy > -2; --dy)					Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::D_DEATH);
	// 안쪽 틀 그리기
	for (dx = 0; dx < m_Width; ++dx)		Print::Instance().InColor(pos.x + dx*2, pos.y - 1, Color::BOX);
	for (dy = 0; dy < m_Height; ++dy)		Print::Instance().InColor(pos.x + dx*2, pos.y + dy, Color::BOX);

	// x 좌표계 그리기
	for (int dx = 0; dx < m_Width; ++dx)
	{
		Print::Instance().InText(pos.x + dx*2 + 1, pos.y + m_Height, CoordTrans(CHAR_X1 + (char)dx));
		Print::Instance().InColor(pos.x + dx * 2, pos.y + m_Height, Color::COORD_BOX);
	}
	// y 좌표계 그리기
	for (int dy = 0; dy < m_Height; ++dy)
	{
		Print::Instance().InText(pos.x - 1, pos.y + dy, CoordTrans(CHAR_Y1 + (char)dy));
		Print::Instance().InColor(pos.x - 2, pos.y + dy, Color::COORD_BOX);
	}

	// 보드 내부 체크무늬 그리기
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
	보드의 위치(pos)와 주인(owner)를 받아서 보드의 현재 상태를 ScreenBuffer에 갱신하는 함수
	- owner가 자신(IAM)인지 적(ENEMY)인지에 따라 히트 처리가 다르다.
*/
void Board::UpdateBoard(Position pos, Owner owner)
{
	if (pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;

	Color color = DEF;
	for (int dy = 0; dy < m_Height; ++dy)
	{
		for (int dx = 0; dx < m_Width; ++dx)
		{
			int shipPart = m_Board[dy][dx];

			// shipPart가 양수면 히트되지 않은 배의 일부분임을 의미
			if (shipPart > 0)
			{
				// 자신의 보드에만 살아있는 배가 표시됨
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
			// shipPart가 음수면 공격이 가해진 좌표임을 의미
			else if (shipPart < 0)
			{
				// 보드의 주인에 따라 히트된 배의 부분을 표시하는 방법이 달라짐
				switch (owner)
				{
				case IAM:	// 자신의 보드에는 히트된 배는 모두 그 배의 색 표시
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

						// 미스인 곳은 타일 색을 그대로 적용
					case MISS:
						if ((dy + dx) % 2 == 1)
							color = TILE_1;
						else
							color = TILE_2;
						break;
					}
					break;
				case ENEMY:	// 적의 보드에는 파괴된 배만 그 배의 색 표시
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
						// 파괴되지 않고, 히트된 부분은 히트 색 표시
					default:
						color = HIT_POS;
						break;
					}
					break;
				}
				Print::Instance().InText(pos.x + dx * 2, pos.y + dy, "[]");
				Print::Instance().InColor(pos.x + dx * 2, pos.y + dy, color);
			}
			// shipPart가 0이면 공격이 가해지지 않은 좌표임을 의미
			else{}
		}
	}
}

/*
	공격받은 좌표(pos)를 받아서 보드의 상태를 갱신하는 함수
*/
void Board::ProcessAttack(Position pos)
{
	int x = pos.x;
	int y = pos.y;

	if (IsInBoard(x, y) == false)
		return;

	// 배가 없는 곳일 경우 MISS 처리
	if (m_Board[y][x] == NO_RESULT)
		m_Board[y][x] = MISS;
	// 배가 있는 곳일 경우 HIT 처리(어떤 배를 나타내는 값에 -1을 곱하면 그 배의 히트됨을 나타냄)
	else
		m_Board[y][x] = -(m_Board[y][x]);
}

/*
	공격받은 좌표(pos)와 공격결과(info)를 받아서 보드의 상태를 갱신하는 함수
	- 네트워크 대전의 경우 상대편 보드의 배 배치상황을 알 수 없으므로 이 함수를 이용한다.
*/
void Board::ProcessAttack(Position pos, HitResult info)
{
	int x = pos.x;
	int y = pos.y;

	if (IsInBoard(x, y) == false)
		return;

	m_Board[y][x] = info;
}

/*
	파괴된 배의 좌표리스트(shipPos)를 받아서 보드의 상태를 갱신하는 함수
*/
void Board::ProcessDestroy(std::vector<Position> shipPos)
{
	// 히트처리된 값에 -10을 하면 파괴됨 처리가 된다.
	for (auto& pos : shipPos)
		m_Board[pos.y][pos.x] -= 10;
}

/*
	배치할 좌표(pos)와 배의 종류(shipType)를 받아서 보드의 상태를 갱신하는 함수
*/
void Board::AddPosition(Position pos, ShipType shipType)
{
	int x = pos.x;
	int y = pos.y;

	if (IsInBoard(x, y) == false)
		return;

	m_Board[y][x] = shipType;
}

/*
	해당 좌표(x,y)에 배가 있으면 true, 없으면 false를 리턴
*/
bool Board::IsShipHere(int x, int y)
{
	if (IsInBoard(x, y) == false)
		return false;

	if (m_Board[y][x] == 0)
		return false;
	else
		return true;
}

/*
	해당 좌표(x,y)가 공격 가능한 곳이면 true, 아니면 false를 리턴
*/
bool Board::IsValidAttack(int x, int y)
{
	if (IsInBoard(x,y) == false)
		return false;

	if (m_Board[y][x] < 0)
		return false;
	else
		return true;
}

/*
	해당 좌표(x,y)가 보드에 존재하면 true, 아니면 false를 리턴
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
	해당 좌표(x,y)가 갇혀있으면 LOWEST_PRIORITY, 아니면 ZERO를 리턴
	- 나중에 다른 계산과 반환값이 추가될 가능성이 있어서 리턴값을 bool로 하지 않음
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
	// 확실하게 배가 존재할 가능성이 없는 경우
	if (outsideNum + missNum + destroyNum > 3)
		return LOWEST_PRIORITY;
	else
		return ZERO;
}

/*
	좌표값(coord)을 받아서 문자열로 변환하여 반환하는 함수
*/
std::string Board::CoordTrans(char coord)
{
	char temp[2];
	temp[0] = coord;
	temp[1] = '\0';
	return temp;
}
