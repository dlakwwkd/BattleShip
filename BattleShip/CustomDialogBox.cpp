#include "stdafx.h"
#include "CustomDialogBox.h"
#include "GameManager.h"
#include "Print.h"
#include "Sound.h"


CustomDialogBox::CustomDialogBox()
{
	msgToken.reserve(MAX_TOKEN_NUM);
}


CustomDialogBox::~CustomDialogBox()
{
}

/*
	대화상자를 깨끗한 상태로 출력하는 함수
*/
void CustomDialogBox::InitDialog()
{
	Decoration();
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X, DIALOG_BOX_POS_Y);
	Print::Instance().SetColor(GREEN);

	// 성능상 좋지않은 로직이지만, 직관적으로 형태를 잡기 위해 이렇게 짰음
	for (int y = 0; y < DIALOG_BOX_SIZE_Y; ++y)
	{
		Print::Instance().Gotoxy(DIALOG_BOX_POS_X, DIALOG_BOX_POS_Y + y);
		for (int x = 0; x < DIALOG_BOX_SIZE_X / 2; ++x)
		{
			if (y == 0 && x == 0)
				printf_s("┌");
			else if (y == 0 && x == DIALOG_BOX_SIZE_X / 2 - 1)
				printf_s("┐");
			else if (y == DIALOG_BOX_SIZE_Y - 1 && x == 0)
				printf_s("└");
			else if (y == DIALOG_BOX_SIZE_Y - 1 && x == DIALOG_BOX_SIZE_X / 2 - 1)
				printf_s("┘");
			else if (x == 0 || x == DIALOG_BOX_SIZE_X / 2 - 1)
				printf_s("│");
			else if (y == 0 || y == DIALOG_BOX_SIZE_Y - 1)
				printf_s("─");
			else
				printf_s("  ");
		}
	}
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 2, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 2);
	printf_s(":> ");
	Print::Instance().SetColor(DEF);
}

/*
	대화상자에서 입력을 받아 메시지 토큰에 저장하는 함수
*/
void CustomDialogBox::InputDialog()
{
	char buf[MAX_STRING_SIZE];
	
	fgets(buf, MAX_STRING_SIZE, stdin);
	if (buf == nullptr)	return;
	
	msgToken.push_back(buf);
}

/*
	메시지 토큰에서 순차적으로 메시지를 꺼내 출력하는 함수
*/
void CustomDialogBox::PrintDialog()
{
	// 입력란 한 칸 위로 커서를 이동시킨다.
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 2, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 3);

	// 토큰에 저장된 메시지를 가장 최근 것부터 한 칸씩 위로 이동해가며 출력한다.
	// 메시지의 첫 글자를 통해 시스템 메시지를 구분하여 색상을 부여한다.
	for (unsigned int i = 0; i < msgToken.size(); ++i)
	{
		Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 2, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 3 - i);
		if (msgToken[msgToken.size() - i - 1][0] == '>')
			Print::Instance().SetColor(SKY);
		else if (msgToken[msgToken.size() - i - 1][0] == '<')
			Print::Instance().SetColor(RED);
		else
			Print::Instance().SetColor(GRAY);
		printf_s("%s", msgToken[msgToken.size() - i - 1].c_str());
	}
	// 메시지가 대화상자 표시 범위를 넘어서면 과거의 것부터 토큰에서 제거한다.
	if (msgToken.size() > MAX_TOKEN_NUM)
		msgToken.erase(msgToken.begin());

	// 커서를 다시 입력란 위치에 이동시킨다.
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 5, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 2);
	Print::Instance().SetColor(DEF);
}

/*
	대화상자에서 입력된 명령 메시지를 처리하는 함수
*/
void CustomDialogBox::ProcessMessage()
{
	// exit 입력시 게임을 종료한다.
	if (msgToken.back() == "exit\n")
	{
		GameManager::Instance().GameOver();
		GameManager::Instance().SetGameOnOff(OFF);
	}
}

/*
	공격결과정보(hit)와 공격주체(player)를 받아서 메시지 토큰에 시스템 메시지를 저장하는 함수
*/
void CustomDialogBox::InputSystemMessage(HitResult hit, int player)
{
	std::string msg = "";
	if (player == PLAYER_1) msg = ">> ";
	else msg = "<< ";
	switch (hit)
	{
	case HIT:
		Sound::Instance().HitSound();
		msg += "Hit!";
		break;
	case MISS:
		Sound::Instance().MissSound();
		msg += "Miss..";
		break;
	case DESTROY_AIRCRAFT:
		Sound::Instance().DestroySound();
		msg += "Destroy Aircraft!!";
		break;
	case DESTROY_BATTLESHIP:
		Sound::Instance().DestroySound();
		msg += "Destroy BattleShip!!";
		break;
	case DESTROY_CRUISER:
		Sound::Instance().DestroySound();
		msg += "Destroy Cruiser!!";
		break;
	case DESTROY_DESTROYER:
		Sound::Instance().DestroySound();
		msg += "Destroy Destroyer!!";
		break;
	}
	msgToken.push_back(msg);
}

/*
	시스템 메시지(msg)를 받아서 메시지 토큰에 저장하는 함수
	- 네트워크 플레이에선 이 함수를 사용한다.
*/
void CustomDialogBox::InputSystemMessage(std::string msg)
{
	msgToken.push_back(msg);
}

/*
	대화상자 꾸미기 함수(입력란을 넘어서는 글자 입력시 그 흔적을 지우는 용도도 있음)
*/
void CustomDialogBox::Decoration()
{
	// 콘솔창 맨 아래에서부터의 높이 설정
	int decoHeight = CONSOLE_LINES - 5;

	Print::Instance().Gotoxy(0, decoHeight);
	Print::Instance().SetColor(DECO);

	for (int y = decoHeight; y < CONSOLE_LINES - 2; ++y)
	{
		// 대화상자 왼쪽 빈 공간 채우기
		for (int x = 0; x < DIALOG_BOX_POS_X; ++x)
			_putch(' ');
		Print::Instance().Gotoxy(DIALOG_BOX_POS_X + DIALOG_BOX_SIZE_X, y);
		// 오른쪽 빈 공간 채우기
		for (int x = DIALOG_BOX_POS_X + DIALOG_BOX_SIZE_X; x < CONSOLE_COLS; ++x)
			_putch(' ');
		Print::Instance().Gotoxy(0, y + 1);
	}
	// 아래쪽 빈 공간 채우기
	for (int i = 0; i < CONSOLE_COLS*2; ++i)
		_putch(' ');
	Print::Instance().SetColor(DEF);
}


