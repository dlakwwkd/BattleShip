#pragma once
#include "stdafx.h"
#include "Enums.h"

#define CONSOLE_LINES		35
#define CONSOLE_COLS		112

#define MY_BOARD_POS_X		30
#define MY_BOARD_POS_Y		10
#define ENEMY_BOARD_POS_X	60
#define ENEMY_BOARD_POS_Y	10

#define MAX_COLOR_LIST		16
#define MAX_COLOR_BUFFER	128

class Print
{
public:
	static Print& Instance()
	{
		static Print instance;
		return instance;
	}
	void Init();
	void InText(int x, int y, std::string text);
	void InColor(int x, int y, Color type);
	void PrintText();
	void Gotoxy(int x, int y);
	void SetColor(int color);

	void SpecialPrint();
	void MenuPrint(int curMenu, std::vector<std::string> menuList);

private:
	Print();
	char m_ScreenBuffer[CONSOLE_LINES + 1][CONSOLE_COLS + 1];
	std::map<Color, std::vector<Position>> m_Color;
};