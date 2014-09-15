#include "stdafx.h"
#include "GameManager.h"
#include "CustomDialogBox.h"
#include "Print.h"
#include "Board.h"
#include "Ship.h" 
#include "Sound.h"
#include "Menu.h"
#include "Human.h"
#include "AI.h"

GameManager::GameManager()
{
	m_DialogBox1		= nullptr;
	m_Player1			= nullptr;
	m_Player2			= nullptr;
	m_BoardPlayer1		= nullptr;
	m_BoardPlayer2		= nullptr;
	m_PlayerType		= AI_PLAYER;
	m_Status			= GAMEOVER;
	m_Turn				= PLAYER_1;
	m_BoardPos[0]		= { 0, 0 };
	m_BoardPos[1]		= { 0, 0 };
	m_MainLoopOn		= ON;
	m_PrintOn			= ON;
	m_GameLoopNum		= 0;
	m_EachGameTurnNum	= 0;
	m_TotalGameTurnNum	= 0;
}


void GameManager::GameRun()
{
	while (m_MainLoopOn)
	{
		MainScreen();
		Menu::Instance().MainMenu();

		if (m_MainLoopOn)
		{
			int lowTurn = MAX_PLAY_NUM;
			int highTurn = 0;
			SetGameLoopNum();
			SetPlayer();
			for (int i = 0; i < m_GameLoopNum;)
			{
				InitGame();
				PlayGameLoop();
				if (!m_MainLoopOn) break;

				lowTurn = m_EachGameTurnNum < lowTurn ? m_EachGameTurnNum : lowTurn;
				highTurn = m_EachGameTurnNum > highTurn ? m_EachGameTurnNum : highTurn;
				Print::Instance().Gotoxy(0, 0);
				printf_s("GamePlay = %d/%d \t\n", ++i, m_GameLoopNum);
				printf_s("PrevGame Turn = %d \t\n", m_EachGameTurnNum);
				printf_s("Lowest Turn = %d \t\n", lowTurn);
				printf_s("Highst Turn = %d \t\n", highTurn);
				printf_s("Average Turn = %f \t\n", (float)m_TotalGameTurnNum / i);
				if (m_PrintOn) getchar();
			}
			DelPlayer();
			CloseGame();
		}
	}
}

void GameManager::MainScreen()
{
	srand((unsigned int)time(NULL));
	system("mode con: lines=36 cols=112");
	system("cls");
	Board tempBoard;
	tempBoard.SetBoardName("<( BattleShip )>");
	tempBoard.PrintBoard({ (CONSOLE_COLS - tempBoard.GetMaxWidth()*3)/2,
		CONSOLE_LINES/2 - tempBoard.GetMaxHeight()});
	
	Print::Instance().SpecialPrint();
	Print::Instance().Init();
	Sound::Instance().StartSound();
}

void GameManager::SetGameLoopNum()
{
	do{
		system("cls");
		Print::Instance().Gotoxy(CONSOLE_COLS / 3, CONSOLE_LINES / 2);
		printf_s("Set Play Number ( 1 ~ 9999 ): ");
		Sound::Instance().MenuMoveSound();
		char temp[5];
		fgets(temp, 5, stdin);
		m_GameLoopNum = atoi(temp);
	} while (m_GameLoopNum < 1 || m_GameLoopNum > MAX_PLAY_NUM);
	fflush(stdin);
	Sound::Instance().MenuEnterSound();
	system("cls");
}

void GameManager::PlayGameLoop()
{
	m_EachGameTurnNum = 0;

	while (m_Status == PLAYING)
	{
		++m_EachGameTurnNum;

		switch (m_Turn)
		{
		case PLAYER_1:
			{
				if (m_PrintOn)
				{
					m_DialogBox1->InitDialog();
					m_DialogBox1->PrintDialog();
				}
				Position attackPosition = m_Player1->Attack();
				m_BoardPlayer2->ProcessAttack(attackPosition);
				HitResult hitResult = m_Player2->DoHitCheck(attackPosition);

				if (m_PrintOn)
					m_DialogBox1->InputSystemMessage(hitResult, PLAYER_1);

				m_Turn = PLAYER_2;
				break;
			}
		case PLAYER_2:
			{
				Position attackPosition = m_Player2->Attack();
				m_BoardPlayer1->ProcessAttack(attackPosition);
				HitResult hitResult = m_Player1->DoHitCheck(attackPosition);

				if (m_PrintOn)
					m_DialogBox1->InputSystemMessage(hitResult, PLAYER_2);

				m_Turn = PLAYER_1;
				break;
			}
		case DONE:
			break;
		}

		if (m_PrintOn)
		{
			m_Player1->GetMyBoard()->UpdateBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y }, IAM);
			m_Player1->GetEnemyBoard()->UpdateBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y }, ENEMY);
			Print::Instance().PrintText();
			Print::Instance().Init();
			m_DialogBox1->InitDialog();
			m_DialogBox1->PrintDialog();
		}
		m_Status = CheckGameStatus();
	}

	if (m_EachGameTurnNum % 2 == 0)
		m_EachGameTurnNum /= 2;
	else
		m_EachGameTurnNum = ((m_EachGameTurnNum + 1) / 2);
	m_TotalGameTurnNum += m_EachGameTurnNum;
}

void GameManager::InitGame()
{
	m_Status = PLAYING;
	m_Turn	 = PLAYER_1;
	m_Player1->InitPlayer();
	m_Player2->InitPlayer();
	if (m_DialogBox1) delete m_DialogBox1;
	if (m_PrintOn)
	{
		Print::Instance().Init();
		Sound::Instance().StartSound();
		m_DialogBox1 = new CustomDialogBox();
		if (m_PlayerType == HUMAN_PLAYER)
		{
			Human* temp = (Human*)m_Player1;
			temp->SetMyDialogBox(m_DialogBox1);
		}
	}
	m_BoardPlayer1 = m_Player1->GetMyBoard();
	m_BoardPlayer2 = m_Player2->GetMyBoard();
	m_Player1->SetEnemyBoard(m_BoardPlayer2);
	m_Player2->SetEnemyBoard(m_BoardPlayer1);
	m_Player1->GetMyBoard()->PrintBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y });
	m_Player1->GetEnemyBoard()->PrintBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y });

	m_Player1->SettingShips();
	m_Player2->SettingShips();

	m_Player1->GetMyBoard()->UpdateBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y }, IAM);
	m_Player1->GetEnemyBoard()->UpdateBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y }, ENEMY);
	
	if (m_PrintOn)
	{
		Print::Instance().PrintText();
		Print::Instance().Init();
		m_DialogBox1->InitDialog();
		m_DialogBox1->PrintDialog();
	}
}

void GameManager::CloseGame()
{
	if (m_PrintOn)
		Print::Instance().PrintText();
	Print::Instance().Init();
	Sound::Instance().CloseSound();
	delete m_DialogBox1;
	m_DialogBox1 = nullptr;
	m_EachGameTurnNum = 0;
	m_TotalGameTurnNum = 0;
	getchar();
}

void GameManager::SetPlayer()
{
	if (m_PlayerType == HUMAN_PLAYER) m_Player1 = new Human();
	else m_Player1 = new AI();
	m_Player2 = new AI();
	m_Player1->SetPlayerName("Player1");
	m_Player2->SetPlayerName("Player2");
	m_Player1->GetMyBoard()->SetBoardName(m_Player1->GetPlayerName());
	m_Player2->GetMyBoard()->SetBoardName(m_Player2->GetPlayerName());
}

void GameManager::DelPlayer()
{
	if (m_Player1->GetPlayerType() == AI_PLAYER)
	{
		AI* temp = (AI*)m_Player1;
		delete temp;
	}
	else
	{
		Human* temp = (Human*)m_Player1;
		delete temp;
	}
	if (m_Player2->GetPlayerType() == AI_PLAYER)
	{
		AI* temp = (AI*)m_Player2;
		delete temp;
	}
	else
	{
		Human* temp = (Human*)m_Player2;
		delete temp;
	}
}

void GameManager::SetBoardPos(Position pos, int num)
{
	if (num<1 || num>2 || pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;
	m_BoardPos[num - 1] = pos;
}

Position GameManager::GetBoardPos(int num)
{
	if (num<1 || num>2)
		return { 0, 0 };
	return m_BoardPos[num];
}

GameStatus GameManager::CheckGameStatus()
{
	if (m_Player1->IsAllSunk()
		|| m_Player2->IsAllSunk()
		|| m_Status == GAMEOVER)
		return GAMEOVER;

	return PLAYING;
}
