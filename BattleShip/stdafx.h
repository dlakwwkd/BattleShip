// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

//#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <crtdbg.h>
#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#ifdef _DEBUG
// Debug�� �����ϴ� ���
#pragma comment(lib, "BGNL_debug.lib")
#else
// Release�� �����ϴ� ���
#pragma comment(lib, "BGNL.lib")
#endif

#include "Network.h"

struct Position
{
	int x;
	int y;
};