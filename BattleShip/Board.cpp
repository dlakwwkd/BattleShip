#include "stdafx.h"
#include "Board.h"
#include "Enums.h"
#include "Print.h"

Board::Board()
{
	m_Name = "UnNamed Board";
	m_Width = 8;
	m_Height = 8;

	m_Board = (int**)malloc(sizeof(int)* m_Height);
	for (int i = 0; i < m_Height; ++i)
	{
		*(m_Board + i) = (int*)malloc(sizeof(int)* m_Width);
	}

	for (int i = 0; i < m_Height; ++i)
	{
		for (int j = 0; j < m_Width; j++)
		{
			m_Board[i][j] = 0;
		}
	}
}


Board::~Board()
{
	for (int i = 0; i < m_Height; ++i)
	{
		free(*(m_Board + i));
	}
	free(m_Board);
}

void Board::PrintBoard(POINT pos)
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

void Board::UpdateBoard(POINT pos, Owner owner)
{
	if (pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;
	Color color = DEF;
	for (int i = 2; i < m_Height + 2; ++i)
	{
		for (int j = 2; j < m_Width + 2; ++j)
		{
			if (m_Board[i - 2][j - 2] > 0)
			{
				switch (owner)
				{
				case IAM:
					switch (m_Board[i - 2][j - 2])
					{
					case 5:
						color = A_LIVE;
						break;
					case 4:
						color = B_LIVE;
						break;
					case 3:
						color = C_LIVE;
						break;
					case 2:
						color = D_LIVE;
						break;
					}
					Print::Instance().InColor(pos.x + j * 2, pos.y + i, color);
					break;
				case ENEMY:
					break;
				}
			}
			else if (m_Board[i - 2][j - 2] < 0)
			{
				switch (owner)
				{
				case IAM:
					switch (m_Board[i - 2][j - 2])
					{
					case -5:
						color = A_DEATH;
						break;
					case -4:
						color = B_DEATH;
						break;
					case -3:
						color = C_DEATH;
						break;
					case -2:
						color = D_DEATH;
						break;
					case -1:
						if ((i + j) % 2 == 1)
							color = TILE_1;
						else
							color = TILE_2;
						break;
					}
					break;
				case ENEMY:
					if (m_Board[i - 2][j - 2] == -1)
					{
						if ((i + j) % 2 == 1)
							color = TILE_1;
						else
							color = TILE_2;
					}
					else
						color = HIT_POS;
					break;
				}
				Print::Instance().InText(pos.x + j * 2, pos.y + i, "[]");
				Print::Instance().InColor(pos.x + j * 2, pos.y + i, color);
			}
		}
	}
}

void Board::AddPosition(int x, int y, int value)
{
	if (MapCheck(x, y) == false)
		return;

	m_Board[x][y] = value;
}

void Board::ProcessAttack(Position pos)
{
	if (MapCheck(pos.m_X, pos.m_Y) == false)
		return;

	if (m_Board[pos.m_X][pos.m_Y] == 0)
		m_Board[pos.m_X][pos.m_Y] = -1;
	else
		m_Board[pos.m_X][pos.m_Y] = -(m_Board[pos.m_X][pos.m_Y]);
}

bool Board::IsShipHere(int x, int y)
{
	if (MapCheck(x, y) == false)
		return false;

	if (m_Board[x][y] == 0) 
		return false;
	else
		return true;
}

bool Board::IsValidAttack(int x, int y)
{
	if (MapCheck(x,y) == false)
		return false;

	if (m_Board[x][y] < 0)
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
