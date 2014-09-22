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
	"�̱� �÷���",
	"��Ƽ �÷���",
	"����",
};
std::vector<std::string> playerMenuList =
{
	"Player vs CPU",
	"CPU vs CPU",
	"AI Test Mode",
	"�ڷ�",
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
	���޵Ǵ� �޴� ���(menuList)�� �޴� ����(menuType)�� ���� �޴��� �����ϴ� �Լ�
	- ���ʿ��� ���縦 �����ϱ� ���� &�� ���޹���
*/
void Menu::MenuPlatform(std::vector<std::string>& menuList, int menuType)
{
	_ASSERT(!(menuList.empty()));
	if (menuList.empty()) return;

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
			// ��� �޴��� ���� �Ʒ��� �ڷΰ��� �Ǵ� ���Ḧ �ǹ�
			if (curMenu == (int)menuList.size() - 1)
			{
				GameManager::Instance().SetGameOnOff(OFF);
				system("cls");
				MenuLoop = OFF;
			}
			// ������ �޴��� �޴� Ÿ�Կ� ���� ������.
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
			// ESC�� �ڷΰ��� �Ǵ� ���Ḧ �ǹ��Ѵ�.
		case ESC_KEY:
			GameManager::Instance().SetGameOnOff(OFF);
			system("cls");
			MenuLoop = OFF;
			break;
		}
	}
}

/*
	���� ����Ű�� �ִ� �޴�(curMenu)�� �� �޴� ����(maxMenuNum)�� �޾� ����Ű�� �޴��� �̵���Ű�� �Լ�
	- curMenu�� ���� �����Ű�� ���� &�� ������, ���ϰ��� ENTER�� ESC ���� Ű�Է��̴�.
*/
int Menu::MenuMove(int& curMenu, int maxMenuNum)
{
	_ASSERT(!(curMenu < 0 || curMenu > maxMenuNum));
	if (curMenu < 0 || curMenu > maxMenuNum) return -1;

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

