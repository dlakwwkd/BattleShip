// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

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
// Debug로 빌드하는 경우
#pragma comment(lib, "BGNL_debug.lib")
#else
// Release로 빌드하는 경우
#pragma comment(lib, "BGNL.lib")
#endif

#include "Network.h"
#include "ShipData.h"

struct Position
{
	int x;
	int y;
};