#include "stdafx.h"
#include "Board.h"
#include "Enums.h"
#include "Print.h"

Board::Board()
{
	m_Name = "UnNamed Board";
	m_Height = 8;
	m_Width = 8;

	m_Board = new int*[m_Height];
	for (int i = 0; i < m_Height; ++i)
	{
		m_Board[i] = new int[m_Width];
	}
	for (int i = 0; i < m_Height; ++i)
	{
		for (int j = 0; j < m_Width; ++j)
		{
			m_Board[i][j] = 0;
		}
	}
}


Board::~Board()
{
	for (int i = 0; i < m_Height; ++i)
	{
		delete m_Board[i];
	}
	delete m_Board;
}


void Board::InitBoard()
{
	for (int i = 0; i < m_Height; ++i)
	{
		for (int j = 0; j < m_Width; ++j)
		{
			m_Board[i][j] = 0;
		}
	}
}

void Board::PrintBoard(Position pos)
{
	if (pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;

	Print::Instance().InText(pos.x + 3, pos.y, m_Name);
	Print::Instance().InColor(pos.x + 2, pos.y + 1, Color(MARK_BOX));
	{
		int i, j;
		for (i = 0; i < m_Width + 2; ++i)
			Print::Instance().InColor(pos.x + i * 2, pos.y, Color(BOX));
		for (j = 1; j < m_Height + 2; ++j)
			Print::Instance().InColor(pos.x + i * 2, pos.y + j, Color(BOX));
		for (; i > 0; --i)
			Print::Instance().InColor(pos.x + i * 2, pos.y + j, Color(BOX));
		for (; j > 0; --j)
			Print::Instance().InColor(pos.x + i * 2, pos.y + j, Color(BOX));
	}
	for (int i = 0; i < m_Height; ++i)
	{
		Print::Instance().InText(pos.x + 3, pos.y + i + 2, CoordTrans(CHAR_Y1 + (char)i));
		Print::Instance().InColor(pos.x + 2, pos.y + i + 2, Color(COORD_BOX));
	}
	for (int i = 0; i < m_Width; ++i)
	{
		Print::Instance().InText(pos.x + (i + 2) * 2 + 1, pos.y + 1, CoordTrans(CHAR_X1 + (char)i));
		Print::Instance().InColor(pos.x + (i + 2) * 2, pos.y + 1, Color(COORD_BOX));
	}
	for (int i = 2; i < m_Height + 2; ++i)
	{
		for (int j = 2; j < m_Width + 2; ++j)
		{
			if ((i + j) % 2 == 1)
				Print::Instance().InColor(pos.x + j * 2, pos.y + i, Color(TILE_1));
			else
				Print::Instance().InColor(pos.x + j * 2, pos.y + i, Color(TILE_2));
		}
	}
}

void Board::UpdateBoard(Position pos, Owner owner)
{
	if (pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;

	Color color = DEF;
	for (int i = 2; i < m_Height + 2; ++i)
	{
		for (int j = 2; j < m_Width + 2; ++j)
		{
			int shipPart = m_Board[i - 2][j - 2];

			if (shipPart > 0)
			{
				switch (owner)
				{
				case IAM:
					switch (shipPart)
					{
					case AIRCRAFT:
						color = A_LIVE;
						break;
					case BATTLESHIP:
						color = B_LIVE;
						break;
					case CRUISER:
						color = C_LIVE;
						break;
					case DESTROYER:
						color = D_LIVE;
						break;
					}
					Print::Instance().InColor(pos.x + j * 2, pos.y + i, color);
					break;
				case ENEMY:
					break;
				}
			}
			else if (shipPart < 0)
			{
				switch (owner)
				{
				case IAM:
					switch (shipPart)
					{
					case DESTROY_AIRCRAFT:
					case -AIRCRAFT:
						color = A_DEATH;
						break;
					case DESTROY_BATTLESHIP:
					case -BATTLESHIP:
						color = B_DEATH;
						break;
					case DESTROY_CRUISER:
					case -CRUISER:
						color = C_DEATH;
						break;
					case DESTROY_DESTROYER:
					case -DESTROYER:
						color = D_DEATH;
						break;
					case MISS:
						if ((i + j) % 2 == 1)
							color = TILE_1;
						else
							color = TILE_2;
						break;
					}
					break;
				case ENEMY:
					switch (shipPart)
					{
					case DESTROY_AIRCRAFT:
						color = A_DEATH;
						break;
					case DESTROY_BATTLESHIP:
						color = B_DEATH;
						break;
					case DESTROY_CRUISER:
						color = C_DEATH;
						break;
					case DESTROY_DESTROYER:
						color = D_DEATH;
						break;
					case MISS:
						if ((i + j) % 2 == 1)
							color = TILE_1;
						else
							color = TILE_2;
						break;
					default:
						color = HIT_POS;
						break;
					}
					break;
				}
				Print::Instance().InText(pos.x + j * 2, pos.y + i, "[]");
				Print::Instance().InColor(pos.x + j * 2, pos.y + i, color);
			}
			else
			{
				Print::Instance().InText(pos.x + j * 2, pos.y + i, "  ");
				if ((i + j) % 2 == 1)
					color = TILE_1;
				else
					color = TILE_2;
			}
		}
	}
}

void Board::ProcessAttack(Position pos)
{
	int x = pos.x;
	int y = pos.y;

	if (MapCheck(x, y) == false)
		return;

	if (m_Board[y][x] == 0)
		m_Board[y][x] = -1;
	else
		m_Board[y][x] = -(m_Board[y][x]);
}

void Board::ProcessAttack(Position pos, HitResult info)
{
	int x = pos.x;
	int y = pos.y;

	if (MapCheck(x, y) == false)
		return;

	m_Board[y][x] = info;
}


void Board::ProcessDestroy(std::vector<Position> shipPos)
{
	for (auto& pos : shipPos)
	{
		m_Board[pos.y][pos.x] -= 10;
	}
}

void Board::AddPosition(int x, int y, ShipType shipType)
{
	if (MapCheck(x, y) == false)
		return;

	m_Board[y][x] = (int)shipType;
}

bool Board::IsShipHere(int x, int y)
{
	if (MapCheck(x, y) == false)
		return false;

	if (m_Board[y][x] == 0)
		return false;
	else
		return true;
}

bool Board::IsValidAttack(int x, int y)
{
	if (MapCheck(x,y) == false)
		return false;

	if (m_Board[y][x] < 0)
		return false;
	else
		return true;
}

bool Board::MapCheck(int x, int y)
{
	if (x < 0 || x >= m_Width ||
		y < 0 || y >= m_Height)
		return false;
	else
		return true;
}

std::string Board::CoordTrans(char coord)
{
	char temp[2];
	temp[0] = coord;
	temp[1] = '\0';
	return temp;
}
