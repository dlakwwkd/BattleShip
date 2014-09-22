#include "stdafx.h"
#include "Print.h"


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

/*
	ScreenBuffer를 초기화 하는 함수
*/
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

/*
	ScreenBuffer에 문자를 기록하는 함수
*/
void Print::InText(int x, int y, std::string text)
{
	_ASSERT(!(x < 0 || y < 0 || x > CONSOLE_COLS || y > CONSOLE_LINES));
	if (x < 0 || y < 0 || x > CONSOLE_COLS || y > CONSOLE_LINES)
		return;

	for (unsigned int i = x; i < x + text.length(); ++i)
		m_ScreenBuffer[y][i] = text[i - x];
}

/*
	map 자료구조에 색상을 저장하는 함수
*/
void Print::InColor(int x, int y, Color type)
{
	_ASSERT(!(x < 0 || y < 0 || x > CONSOLE_COLS || y > CONSOLE_LINES));
	if (x < 0 || y < 0 || x > CONSOLE_COLS || y > CONSOLE_LINES)
		return;

	m_Color[type].push_back({ x, y });
}

/*
	map에 저장된 색상별로 순서대로 꺼내 화면에 출력하는 함수
*/
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

/*
	커서 이동 함수
*/
void Print::Gotoxy(int x, int y)
{
	COORD pos = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/*
	cmd 색상 변경 함수
*/
void Print::SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}

/*
	대기화면의 애니메이션 출력을 위해 적절히 Sleep을 걸어둔 함수
	- 키입력이 들어오거나 출력이 끝나면 종료
*/
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
				Sleep(60);
				_putch(m_ScreenBuffer[color.second.back().y][color.second.back().x + 1]);

				Gotoxy(CONSOLE_COLS * 3 / 7, CONSOLE_LINES * 2 / 3);
				SetColor(rand() % 15);
				printf_s("- Press any key -");
				Sleep(120);

				SetColor(Color(color.first));
				color.second.pop_back();

				if (_kbhit())
					return;
			}
		}
	}
}

/*
	현재 가르키고 있는 메뉴(curMenu)와 메뉴 목록(menuList)를 받아 색상을 구분해 출력하는 함수
	- 불필요한 복사를 막기위해 &를 사용함
*/
void Print::MenuPrint(int curMenu, std::vector<std::string>& menuList)
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
