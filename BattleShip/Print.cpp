#include "stdafx.h"
#include "Print.h"
#include "Enums.h"

Print::Print()
{
	int x, y;
	for (y = 0; y < CONSOLE_LINES; ++y)
	{
		for (x = 0; x < CONSOLE_COLS; ++x)
			m_ScreenBuffer[y][x] = ' ';
		m_ScreenBuffer[y][x] = '\0';
	}
}

void Print::Init()
{
	int x, y;
	for (y = 0; y < CONSOLE_LINES; ++y)
	{
		for (x = 0; x < CONSOLE_COLS; ++x)
			m_ScreenBuffer[y][x] = ' ';
		m_ScreenBuffer[y][x] = '\0';
	}
	m_Color.clear();
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


void Print::SpecialPrint()
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
				Sleep(50);
				_putch(m_ScreenBuffer[color.second.back().y][color.second.back().x + 1]);

				Gotoxy(CONSOLE_COLS*3/7, CONSOLE_LINES*2 / 3);
				SetColor(rand() % 15);
				printf_s("Press any key...");
				Sleep(200);

				SetColor(Color(color.first));
				color.second.pop_back();

				if (_kbhit())
				{
					getchar();
					return;
				}
			}
		}
	}
	getchar();
}

void Print::MenuPrint(int curMenu, std::vector<std::string> menuList)
{
	int i = 0;
	for (auto& menu : menuList)
	{
		Gotoxy(CONSOLE_COLS * 2 / 5, CONSOLE_LINES * 2 / 5 + i);
		if(curMenu == i++) SetColor(GREEN);
		else SetColor(DEF);

		for (unsigned int j = 10; j > menu.length()/2; --j)
			printf_s(" ");
		printf_s("[ %s ]", menu.c_str());
	}
	SetColor(DEF);
}
