#include "stdafx.h"
#include "CustomDialogBox.h"
#include "GameManager.h"
#include "Print.h"
#include "Enums.h"
#include "Sound.h"

CustomDialogBox::CustomDialogBox()
{
	msgToken.reserve(MAX_TOKEN_NUM);
}


CustomDialogBox::~CustomDialogBox()
{
}


void CustomDialogBox::InitDialog()
{
	Decoration();
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X, DIALOG_BOX_POS_Y);
	Print::Instance().SetColor(GREEN);

	for (int i = 0; i < DIALOG_BOX_SIZE_Y; ++i)
	{
		Print::Instance().Gotoxy(DIALOG_BOX_POS_X, DIALOG_BOX_POS_Y + i);
		for (int j = 0; j < DIALOG_BOX_SIZE_X / 2; ++j)
		{
			if (i == 0 && j == 0)
				printf_s("¦£");
			else if (i == 0 && j == DIALOG_BOX_SIZE_X / 2 - 1)
				printf_s("¦¤");
			else if (i == DIALOG_BOX_SIZE_Y - 1 && j == 0)
				printf_s("¦¦");
			else if (i == DIALOG_BOX_SIZE_Y - 1 && j == DIALOG_BOX_SIZE_X / 2 - 1)
				printf_s("¦¥");
			else if (j == 0 || j == DIALOG_BOX_SIZE_X / 2 - 1)
				printf_s("¦¢");
			else if (i == 0 || i == DIALOG_BOX_SIZE_Y - 1)
				printf_s("¦¡");
			else
				printf_s("  ");
		}
	}
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 2, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 2);
	printf_s(":> ");
	Print::Instance().SetColor(DEF);
}

void CustomDialogBox::InputDialog()
{
	char buf[MAX_STRING_SIZE];
	
	fgets(buf, MAX_STRING_SIZE, stdin);
	if (buf == nullptr)	return;
	
	msgToken.push_back(buf);
}

void CustomDialogBox::PrintDialog()
{
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 2, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 3);
	Print::Instance().SetColor(GRAY);

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
	if (msgToken.size() > MAX_TOKEN_NUM)
		msgToken.erase(msgToken.begin());

	Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 5, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 2);
	Print::Instance().SetColor(DEF);
}

void CustomDialogBox::ProcessMessage()
{
	if (msgToken.back() == "exit\n")
	{
		GameManager::Instance().GameOver();
		GameManager::Instance().SetGameOnOff(OFF);
	}
}

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

void CustomDialogBox::InputSystemMessage(std::string msg)
{
	msgToken.push_back(msg);
}


void CustomDialogBox::Decoration()
{
	Print::Instance().Gotoxy(0, CONSOLE_LINES - 7);
	Print::Instance().SetColor(DECO);
	for (int j = 0; j < 6; ++j)
	{
		for (int i = 0; i < DIALOG_BOX_POS_X; ++i)
		{
			_putch(' ');
		}
		Print::Instance().Gotoxy(DIALOG_BOX_POS_X + DIALOG_BOX_SIZE_X, CONSOLE_LINES - 7 + j);
		for (int i = 0; i < DIALOG_BOX_POS_X; ++i)
		{
			_putch(' ');
		}
		Print::Instance().Gotoxy(0, CONSOLE_LINES - 7 + j);
	}
	for (int i = 0; i < 224; ++i)
	{
		_putch(' ');
	}
	Print::Instance().SetColor(DEF);
}


