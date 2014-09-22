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
	��ȭ���ڸ� ������ ���·� ����ϴ� �Լ�
*/
void CustomDialogBox::InitDialog()
{
	Decoration();
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X, DIALOG_BOX_POS_Y);
	Print::Instance().SetColor(GREEN);

	// ���ɻ� �������� ����������, ���������� ���¸� ��� ���� �̷��� ®��
	for (int y = 0; y < DIALOG_BOX_SIZE_Y; ++y)
	{
		Print::Instance().Gotoxy(DIALOG_BOX_POS_X, DIALOG_BOX_POS_Y + y);
		for (int x = 0; x < DIALOG_BOX_SIZE_X / 2; ++x)
		{
			if (y == 0 && x == 0)
				printf_s("��");
			else if (y == 0 && x == DIALOG_BOX_SIZE_X / 2 - 1)
				printf_s("��");
			else if (y == DIALOG_BOX_SIZE_Y - 1 && x == 0)
				printf_s("��");
			else if (y == DIALOG_BOX_SIZE_Y - 1 && x == DIALOG_BOX_SIZE_X / 2 - 1)
				printf_s("��");
			else if (x == 0 || x == DIALOG_BOX_SIZE_X / 2 - 1)
				printf_s("��");
			else if (y == 0 || y == DIALOG_BOX_SIZE_Y - 1)
				printf_s("��");
			else
				printf_s("  ");
		}
	}
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 2, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 2);
	printf_s(":> ");
	Print::Instance().SetColor(DEF);
}

/*
	��ȭ���ڿ��� �Է��� �޾� �޽��� ��ū�� �����ϴ� �Լ�
*/
void CustomDialogBox::InputDialog()
{
	char buf[MAX_STRING_SIZE];
	
	fgets(buf, MAX_STRING_SIZE, stdin);
	if (buf == nullptr)	return;
	
	msgToken.push_back(buf);
}

/*
	�޽��� ��ū���� ���������� �޽����� ���� ����ϴ� �Լ�
*/
void CustomDialogBox::PrintDialog()
{
	// �Է¶� �� ĭ ���� Ŀ���� �̵���Ų��.
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 2, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 3);

	// ��ū�� ����� �޽����� ���� �ֱ� �ͺ��� �� ĭ�� ���� �̵��ذ��� ����Ѵ�.
	// �޽����� ù ���ڸ� ���� �ý��� �޽����� �����Ͽ� ������ �ο��Ѵ�.
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
	// �޽����� ��ȭ���� ǥ�� ������ �Ѿ�� ������ �ͺ��� ��ū���� �����Ѵ�.
	if (msgToken.size() > MAX_TOKEN_NUM)
		msgToken.erase(msgToken.begin());

	// Ŀ���� �ٽ� �Է¶� ��ġ�� �̵���Ų��.
	Print::Instance().Gotoxy(DIALOG_BOX_POS_X + 5, DIALOG_BOX_POS_Y + DIALOG_BOX_SIZE_Y - 2);
	Print::Instance().SetColor(DEF);
}

/*
	��ȭ���ڿ��� �Էµ� ��� �޽����� ó���ϴ� �Լ�
*/
void CustomDialogBox::ProcessMessage()
{
	// exit �Է½� ������ �����Ѵ�.
	if (msgToken.back() == "exit\n")
	{
		GameManager::Instance().GameOver();
		GameManager::Instance().SetGameOnOff(OFF);
	}
}

/*
	���ݰ������(hit)�� ������ü(player)�� �޾Ƽ� �޽��� ��ū�� �ý��� �޽����� �����ϴ� �Լ�
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
	�ý��� �޽���(msg)�� �޾Ƽ� �޽��� ��ū�� �����ϴ� �Լ�
	- ��Ʈ��ũ �÷��̿��� �� �Լ��� ����Ѵ�.
*/
void CustomDialogBox::InputSystemMessage(std::string msg)
{
	msgToken.push_back(msg);
}

/*
	��ȭ���� �ٹ̱� �Լ�(�Է¶��� �Ѿ�� ���� �Է½� �� ������ ����� �뵵�� ����)
*/
void CustomDialogBox::Decoration()
{
	// �ܼ�â �� �Ʒ����������� ���� ����
	int decoHeight = CONSOLE_LINES - 5;

	Print::Instance().Gotoxy(0, decoHeight);
	Print::Instance().SetColor(DECO);

	for (int y = decoHeight; y < CONSOLE_LINES - 2; ++y)
	{
		// ��ȭ���� ���� �� ���� ä���
		for (int x = 0; x < DIALOG_BOX_POS_X; ++x)
			_putch(' ');
		Print::Instance().Gotoxy(DIALOG_BOX_POS_X + DIALOG_BOX_SIZE_X, y);
		// ������ �� ���� ä���
		for (int x = DIALOG_BOX_POS_X + DIALOG_BOX_SIZE_X; x < CONSOLE_COLS; ++x)
			_putch(' ');
		Print::Instance().Gotoxy(0, y + 1);
	}
	// �Ʒ��� �� ���� ä���
	for (int i = 0; i < CONSOLE_COLS*2; ++i)
		_putch(' ');
	Print::Instance().SetColor(DEF);
}


