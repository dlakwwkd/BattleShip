#include "stdafx.h"
#include "GameManager.h"
#include "CustomDialogBox.h"
#include "Player.h"
#include "Print.h"
#include "Board.h"
#include "Ship.h" 
#include "Sound.h"

GameManager::GameManager()
{
	m_DialogBox1		= nullptr;
	m_Player1			= nullptr;
	m_Player2			= nullptr;
	m_BoardPlayer1		= nullptr;
	m_BoardPlayer2		= nullptr;
	m_Status			= GAMEOVER;
	m_Turn				= PLAYER_1;
	m_BoardPos[0]		= { 0, 0 };
	m_BoardPos[1]		= { 0, 0 };
	m_MainLoopOn		= true;
	m_GameLoopNum		= 3;
	m_EachGameTurnNum	= 0;
	m_TotalGameTurnNum	= 0;
}


void GameManager::GameRun()
{
	while (m_MainLoopOn)
	{
		MainScreen();
		MainMenu();

		SetPlayer();
		for (int i = 0; i < m_GameLoopNum; ++i)
		{
			InitGame();
			PlayGameLoop();
		}
		Print::Instance().Gotoxy(0, 0);
		printf_s("TotalGameTurnNum = %d\n", m_TotalGameTurnNum);
		printf_s("AverageTotalTurnNum = %f\n", (float)m_TotalGameTurnNum / m_GameLoopNum);

		DelPlayer();
		CloseGame();
	}

}

void GameManager::MainScreen()
{
	srand((unsigned int)time(NULL));
	system("mode con: lines=36 cols=112");
	system("cls");
	Board tempBoard;
	tempBoard.PrintBoard({ (CONSOLE_COLS - tempBoard.GetMaxWidth()*3)/2,
		CONSOLE_LINES/2 - tempBoard.GetMaxHeight()});
	
	Print::Instance().SpecialPrint();
	Print::Instance().Init();
}

void GameManager::MainMenu()
{

}

void GameManager::PlayGameLoop()
{
	m_EachGameTurnNum = 0;
	while (m_Status == PLAYING)
	{
		++m_EachGameTurnNum;
		Print::Instance().PrintText();
		Print::Instance().Init();
		m_DialogBox1->InitDialog();
		m_DialogBox1->PrintDialog();
		//m_DialogBox1->InputDialog();
		//m_DialogBox1->ProcessMessage();

		switch (m_Turn)
		{
		case PLAYER_1:
			{
				POINT attackPosition = m_Player1->Attack();
				m_BoardPlayer2->ProcessAttack(attackPosition);

				HitResult hitResult = m_Player2->DoHitCheck(attackPosition);
				m_Player1->SetPrevAttackInfo({ attackPosition, hitResult });
				m_DialogBox1->InputSystemMessage(hitResult);
				m_Turn = PLAYER_2;
				break;
			}
		case PLAYER_2:
			{
				POINT attackPosition = m_Player2->Attack();
				m_BoardPlayer1->ProcessAttack(attackPosition);

				HitResult hitResult = m_Player1->DoHitCheck(attackPosition);
				m_Player2->SetPrevAttackInfo({ attackPosition, hitResult });
				m_DialogBox1->InputSystemMessage(hitResult);
				m_Turn = PLAYER_1;
				break;
			}
		case DONE:
			break;
		}
		m_Player1->GetMyBoard()->UpdateBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y }, IAM);
		m_Player1->GetEnemyBoard()->UpdateBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y }, ENEMY);

		m_Status = CheckGameStatus();
	}

	if (m_EachGameTurnNum % 2 == 0)
	{
		m_TotalGameTurnNum += (m_EachGameTurnNum / 2);
	}
	else
	{
		m_TotalGameTurnNum += ((m_EachGameTurnNum+1) / 2);
	}

}

void GameManager::InitGame()
{
	system("cls");
	m_Status = PLAYING;
	m_Turn	 = PLAYER_1;
	m_Player1->InitPlayer();
	m_Player2->InitPlayer();
	if (m_DialogBox1) delete m_DialogBox1;

	Print::Instance().Init();
	Sound::Instance().StartSound();

	// 각 플레이어가 배들을 배치하도록 함
	m_Player1->SettingShips();
	m_Player2->SettingShips();

	// 각 플레이어가 자신의 보드를 제출하고, 적의 보드를 인지하도록 함
	m_BoardPlayer1 = m_Player1->GetMyBoard();
	m_BoardPlayer2 = m_Player2->GetMyBoard();
	m_Player1->SetEnemyBoard(m_BoardPlayer2);
	m_Player2->SetEnemyBoard(m_BoardPlayer1);

	// 플레이어 1의 화면을 출력
	m_Player1->GetMyBoard()->PrintBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y });
	m_Player1->GetEnemyBoard()->PrintBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y });
	m_Player1->GetMyBoard()->UpdateBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y }, IAM);
	m_Player1->GetEnemyBoard()->UpdateBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y }, ENEMY);

	// 대화상자 생성
	m_DialogBox1 = new CustomDialogBox();
}

void GameManager::CloseGame()
{
	Print::Instance().PrintText();
	Print::Instance().Init();
	Sound::Instance().CloseSound();
	delete m_DialogBox1;
	m_DialogBox1 = nullptr;
	getchar();
}

void GameManager::SetPlayer()
{
	//임시 객체생성(나중엔 플레이어를 따로 받을 예정)
	m_Player1 = new Player();	
	m_Player2 = new Player();
	m_Player1->SetPlayerName("Player1");
	m_Player2->SetPlayerName("Player2");
	m_Player1->GetMyBoard()->SetBoardName(m_Player1->GetPlayerName());
	m_Player2->GetMyBoard()->SetBoardName(m_Player2->GetPlayerName());
}

void GameManager::DelPlayer()
{
	delete m_Player1;
	delete m_Player2;
}

void GameManager::SetBoardPos(POINT pos, int num)
{
	if (num<1 || num>2 || pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;
	m_BoardPos[num - 1] = pos;
}

POINT GameManager::GetBoardPos(int num)
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
