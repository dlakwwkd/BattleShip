// BattleShip_pp.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <windows.h>

std::vector<std::string> units;
std::vector<std::string> checkUnits;

int _tmain(int argc, _TCHAR* argv[])
{
	//UNREFERENCED_PARAMETER(argc);
	argc;
	argv;
	units.push_back("Aircraft");
	units.push_back("Battleship");
	units.push_back("Battleship");
	units.push_back("Cruiser");
	units.push_back("Destroyer");
	units.push_back("Destroyer");

	std::string input_str;
	std::string guess_request = "좌표 입력: ";
	std::string respone[] = { "HIT", "MISS", "DESTROY" };

	srand(time(NULL));

	while (!(units.empty()))
	{
		system("cls");
		for (int i = 0; i < units.size(); i++)
		{
			std::cout << units[i] << "   ";
		}
		std::cout << std::endl;

		std::cout << guess_request;
		std::cin >> input_str;

		if (input_str == "quit") break;
		int random = rand() % _countof(respone);
		int randPick = rand() % units.size();

		switch (random)
		{
		case 0:
			std::cout << respone[random] << std::endl;
			break;
		case 1:
			std::cout << respone[random] << std::endl;
			break; 
		case 2:
			std::cout << units[randPick] << " is DESTROYED!" << std::endl;
			units.erase(units.begin() + randPick);
			break;
		}
		std::cout << "\nNext Turn!" << std::endl;
		getchar();
		getchar();
	}
	std::cout << "You win!" << std::endl;
	getchar();

	return 0;
}

