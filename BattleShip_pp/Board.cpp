#include "stdafx.h"
#include "Board.h"
#include "Enums.h"

Board::Board()
{
	m_Width = 8;
	m_Height = 8;
	m_Name = "UnNamed Board";

	m_Board = (int**)malloc(sizeof(int)* m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		*(m_Board + i) = (int*)malloc(sizeof(int)* m_Width);
	}

	// init board
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			m_Board[i][j] = 0;
		}
	}

	//PrintBoard();
}


Board::~Board()
{
	for (int i = 0; i < m_Height; i++)
	{
		free(*(m_Board + i));
	}
	free(m_Board);
}

void Board::AddPosition(int x, int y, int value)
{
	if (MapCheck(x, y) == false)
		return;

	//printf_s("Adding pos %d, %d to board\n", x, y);
		
	m_Board[x][y] = value;
}

void Board::PrintBoard()
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	

	printf_s("\n[ BOARD : %s ]\n\n", m_Name.c_str());
	
	for (int i = 0; i <= m_Height; i++)
	{
		for (int j = 0; j <= m_Width; j++)
		{
			SetConsoleTextAttribute(hConsole, 7);

			if (i == 0)
			{
				SetConsoleTextAttribute(hConsole, 11);

				if (j == 0) printf_s("\t");
				else printf_s("%3d", j);

				if (j == m_Width)
				{
					printf_s("\n");
					for (int k = 0; k <= m_Width; k++)
						printf_s("----");
				}
			}
			else if (j == 0)  // && i == 0
			{
				SetConsoleTextAttribute(hConsole, 11);
				printf_s("%c  |\t", 'A' - 1 + i);
			}
			else
			{
				if (m_Board[i - 1][j - 1] > 0)				
					SetConsoleTextAttribute(hConsole, 14);
				else if (m_Board[i - 1][j - 1] < 0)
					SetConsoleTextAttribute(hConsole, 12);
				printf_s("%3d", m_Board[i - 1][j - 1]);
			}
		}
		printf_s("\n");
	}
	SetConsoleTextAttribute(hConsole, 7);
	printf_s("\n");
}

void Board::ProcessAttack(Position pos)
{
	if (m_Board[pos.m_X][pos.m_Y] == 0)
		m_Board[pos.m_X][pos.m_Y] = -1;
	else
		m_Board[pos.m_X][pos.m_Y] = -(m_Board[pos.m_X][pos.m_Y]);
}

bool Board::DuplCheck(int x, int y)
{
	if (m_Board[x][y] < 0)
		return false;
	else
		return true;
}


bool Board::MapCheck(int posX, int posY)
{
	if (posX < 0 || posX >= m_Width
		|| posY < 0 || posY >= m_Height)
		return false;
	else
		return true;
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
