// BattleShip_pp.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
	std::string guess_request = "��ǥ �Է�: ";
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

