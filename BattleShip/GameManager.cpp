#include "stdafx.h"
#include "GameManager.h"
#include "CustomDialogBox.h"
#include "Player.h"
#include "Print.h"
#include "Board.h"
#include "Ship.h" 
#include "Setting.h"
#include "Sound.h"

GameManager::GameManager()
{
	m_Player1		= nullptr;
	m_Player2		= nullptr;
	m_Board_Player1 = nullptr;
	m_Board_Player2 = nullptr;
	m_Status		= GAMEOVER;
	m_Turn			= PLAYER_1;
}


void GameManager::GameRun()
{
	InitGame();
	PlayGameLoop();
	CloseGame();
}

void GameManager::PlayGameLoop()
{
	while (m_Status == PLAYING)
	{
		Print::Instance().PrintText();
		Print::Instance().Init();
		m_DialogBox->InitDialog();
		m_DialogBox->PrintDialog();
		m_DialogBox->InputDialog();
		m_DialogBox->ProcessMessage();

		switch (m_Turn)
		{
		case PLAYER_1:
			{
				Position attackPosition = m_Player1->Attack();
				HitResult hitResult = m_Player2->DoHitCheck(attackPosition);
				m_Player1->ProcessHitResult(hitResult);
				m_DialogBox->InputSystemMessage(hitResult);
				m_Board_Player2->ProcessAttack(attackPosition);
				m_Turn = PLAYER_2;
				break;
			}
		case PLAYER_2:
			{
				Position attackPosition = m_Player2->Attack();
				HitResult hitResult = m_Player1->DoHitCheck(attackPosition);
				m_Player2->ProcessHitResult(hitResult);
				m_DialogBox->InputSystemMessage(hitResult);
				m_Board_Player1->ProcessAttack(attackPosition);
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
}

void GameManager::InitGame()
{
	srand((unsigned int)time(NULL));
	system("mode con: lines=36 cols=112");
	m_Status = PLAYING;
	m_Turn	 = PLAYER_1;
	//Sound::Instance().StartSound();
	SetPlayer();

	// 각 플레이어가 배들을 배치하도록 함
	m_Player1->SetupShips();
	m_Player2->SetupShips();

	// 각 플레이어가 자신의 보드를 제출하고, 적의 보드를 인지하도록 함
	m_Board_Player1 = m_Player1->GetMyBoard();
	m_Board_Player2 = m_Player2->GetMyBoard();
	m_Player1->SetEnemyBoard(m_Board_Player2);
	m_Player2->SetEnemyBoard(m_Board_Player1);

	// 플레이어 1의 화면을 출력
	m_Player1->GetMyBoard()->PrintBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y });
	m_Player1->GetEnemyBoard()->PrintBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y });
	m_Player1->GetMyBoard()->UpdateBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y }, IAM);
	m_Player1->GetEnemyBoard()->UpdateBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y }, ENEMY);

	// 대화상자 출력
	m_DialogBox = new CustomDialogBox();
}

void GameManager::CloseGame()
{
	Print::Instance().PrintText();
	Sound::Instance().CloseSound();
	getchar();
	DelPlayer();
	delete m_DialogBox;
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
	m_Board_Pos[num - 1] = pos;
}

POINT GameManager::GetBoardPos(int num)
{
	if (num<1 || num>2)
		return { 0, 0 };
	return m_Board_Pos[num];
}

GameStatus GameManager::CheckGameStatus()
{
	if (m_Player1->IsAllSunk()
		|| m_Player2->IsAllSunk()
		|| m_Status == GAMEOVER)
		return GAMEOVER;

	return PLAYING;
}
