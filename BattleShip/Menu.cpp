#include "stdafx.h"
#include "Menu.h"
#include "Print.h"
#include "Sound.h"
#include "GameManager.h"
#include "Enums.h"

Menu::Menu()
{
}

std::vector<std::string> mainMenuList = 
{
	"싱글 플레이",
	"멀티 플레이",
	"옵션",
	"종료",
};
std::vector<std::string> playerMenuList =
{
	"Player vs CPU",
	"CPU vs CPU",
	"AI Test Mode",
	"뒤로",
};

void Menu::MainMenu()
{
	MenuPlatform(mainMenuList, MAIN_MENU);
}

void Menu::PlayerMenu()
{
	MenuPlatform(playerMenuList, PlAYER_MENU);
}

void Menu::MenuPlatform(std::vector<std::string>& menuList, int menuType)
{
	system("cls");
	int curMenu = 0;
	int input = 0;
	bool MenuLoop = ON;
	while (MenuLoop)
	{
		Print::Instance().MenuPrint(curMenu, menuList);
		input = MenuMove(curMenu, menuList.size());
		switch (input)
		{
		case ENTER_KEY:
			if (curMenu == (int)menuList.size() - 1)
			{
				GameManager::Instance().SetGameOnOff(OFF);
				system("cls");
				MenuLoop = OFF;
			}
			else
			{
				switch (menuType)
				{
				case MAIN_MENU:
					switch (curMenu)
					{
					case 0:
						PlayerMenu();
						if (GameManager::Instance().GetMainLoopStatus() == ON)
							return;
						break;
					}
					break;
				case PlAYER_MENU:
					switch (curMenu)
					{
					case 0:
						GameManager::Instance().SetPlayerType(HUMAN_PLAYER);
						GameManager::Instance().SetGameOnOff(ON);
						GameManager::Instance().SetPrintOnOff(ON);
						system("cls");
						MenuLoop = OFF;
						break;
					case 1:
						GameManager::Instance().SetPlayerType(AI_PLAYER);
						GameManager::Instance().SetGameOnOff(ON);
						GameManager::Instance().SetPrintOnOff(ON);
						system("cls");
						MenuLoop = OFF;
						break;
					case 2:
						GameManager::Instance().SetGameOnOff(ON);
						GameManager::Instance().SetPrintOnOff(OFF);
						system("cls");
						MenuLoop = OFF;
						break;
					}
					break;
				}
			}
			break;
		case ESC_KEY:
			GameManager::Instance().SetGameOnOff(OFF);
			system("cls");
			MenuLoop = OFF;
			break;
		}
	}
}

int Menu::MenuMove(int& curMenu, int maxMenuNum)
{
	int input = _getch();
	switch (input)
	{
	case SCAN_CODE:
		input = _getch();
		switch (input)
		{
		case UP_KEY:
			if (curMenu < 1) curMenu = maxMenuNum - 1;
			else --curMenu;
			break;
		case DOWN_KEY:
			if (curMenu > maxMenuNum - 2) curMenu = 0;
			else ++curMenu;
			break;
		}
		Sound::Instance().MenuMoveSound();
		break;
	case ENTER_KEY:
		Sound::Instance().MenuEnterSound();
		return ENTER_KEY;
	case ESC_KEY:
		Sound::Instance().MenuESCSound();
		return ESC_KEY;
	}
	return 0;
}

