#include "stdafx.h"
#include "Menu.h"
#include "Print.h"
#include "Sound.h"
#include "GameManager.h"


Menu::Menu()
{
}

std::vector<std::string> mainMenuList = 
{
	"싱글 플레이",
	"멀티 플레이",
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

/*
	전달되는 메뉴 목록(menuList)과 메뉴 종류(menuType)에 따라 메뉴를 구성하는 함수
	- 불필요한 복사를 방지하기 위해 &로 전달받음
*/
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
			// 모든 메뉴의 가장 아래는 뒤로가기 또는 종료를 의미
			if (curMenu == (int)menuList.size() - 1)
			{
				GameManager::Instance().SetGameOnOff(OFF);
				system("cls");
				MenuLoop = OFF;
			}
			// 나머지 메뉴는 메뉴 타입에 따라 나뉜다.
			else
			{
				switch (menuType)
				{
				case MAIN_MENU:
					switch (curMenu)
					{
					case 0:
						GameManager::Instance().SetGameMode(SINGLE_PLAY);
						break;
					case 1:
						GameManager::Instance().SetGameMode(NETWORK_PLAY);
						break;
					}
					PlayerMenu();
					if (GameManager::Instance().GetMainLoopStatus() == ON)
						return;
					break;
				case PlAYER_MENU:
					switch (curMenu)
					{
					case 0:
						GameManager::Instance().SetPlayerType(HUMAN_PLAYER);
						GameManager::Instance().SetGameOnOff(ON);
						GameManager::Instance().SetPrintOnOff(ON);
						break;
					case 1:
						GameManager::Instance().SetPlayerType(AI_PLAYER);
						GameManager::Instance().SetGameOnOff(ON);
						GameManager::Instance().SetPrintOnOff(ON);
						break;
					case 2:
						GameManager::Instance().SetGameOnOff(ON);
						GameManager::Instance().SetPrintOnOff(OFF);
						break;
					}
					system("cls");
					MenuLoop = OFF;
					break;
				}
			}
			break;
			// ESC는 뒤로가기 또는 종료를 의미한다.
		case ESC_KEY:
			GameManager::Instance().SetGameOnOff(OFF);
			system("cls");
			MenuLoop = OFF;
			break;
		}
	}
}

/*
	현재 가르키고 있는 메뉴(curMenu)와 총 메뉴 갯수(maxMenuNum)를 받아 가르키는 메뉴를 이동시키는 함수
	- curMenu를 직접 변경시키기 위해 &로 받으며, 리턴값은 ENTER나 ESC 등의 키입력이다.
*/
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

