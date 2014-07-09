// BattleShip_pp.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>

int _tmain(int argc, _TCHAR* argv[])
{
	//UNREFERENCED_PARAMETER(argc);
	argc;
	argv;

	std::string input_str;
	std::string guess_request = "좌표 입력: ";
	std::string respone[] = { "HIT", "MISS", "DESTROY" };
	
	srand(time(NULL));

	while (1)
	{
		std::cout << guess_request;
		std::cin >> input_str;

		if (input_str == "quit") break;

		int random = rand() % 3;

		std::cout << respone[random] << std::endl;

	}

	
	return 0;
}

