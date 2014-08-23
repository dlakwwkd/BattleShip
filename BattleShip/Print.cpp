#include "stdafx.h"
#include "Print.h"
#include "Enums.h"

Print::Print()
{
	int x, y;
	for (y = 0; y < CONSOLE_LINES; y++)
	{
		for (x = 0; x < CONSOLE_COLS; x++)
			m_ScreenBuffer[y][x] = ' ';
		m_ScreenBuffer[y][x] = '\0';
	}
}

void Print::Init()
{
	int x, y;
	for (y = 0; y < CONSOLE_LINES; y++)
	{
		for (x = 0; x < CONSOLE_COLS; x++)
			m_ScreenBuffer[y][x] = ' ';
		m_ScreenBuffer[y][x] = '\0';
	}
	SetColor(Color(DEF));
}

void Print::InText(int x, int y, std::string text)
{
	if (x < 0 || y < 0 || x > CONSOLE_COLS || y > CONSOLE_LINES) return;
	for (unsigned int i = x; i < x + text.length(); ++i)
		m_ScreenBuffer[y][i] = text[i - x];
}

void Print::InColor(int x, int y, Color type)
{
	if (x < 0 || y < 0 || x > CONSOLE_COLS || y > CONSOLE_LINES) return;
	m_Color[type].push_back({ x, y });
}

void Print::PrintText()
{
	for (auto& color : m_Color)
	{
		if (!color.second.empty())
		{
			SetColor(Color(color.first));
			while (!color.second.empty())
			{
				Gotoxy(color.second.back().x, color.second.back().y);
				_putch(m_ScreenBuffer[color.second.back().y][color.second.back().x]);
				_putch(m_ScreenBuffer[color.second.back().y][color.second.back().x + 1]);
				color.second.pop_back();
			}
		}
	}
}

void Print::Gotoxy(int x, int y)
{
	COORD pos = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Print::SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}
