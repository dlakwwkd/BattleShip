#include "stdafx.h"
#include "GameManager.h"
#include "Player.h"
#include "Board.h"
#include "Ship.h" 
#include "Position.h"

GameManager::GameManager()
{
	m_Status = GAMEOVER;
	m_Turn = ATTACKER;
}


GameManager::~GameManager()
{
}

void GameManager::StartGame()
{
	m_Attacker = new Player();
	m_Defender = new Player();
	m_Board_Attacker = m_Attacker->GetMyBoard();
	m_Board_Defender = m_Defender->GetEnemyBoard();

	m_Attacker->SetupShips();
	m_Defender->SetupShips();

	m_Status = PLAYING;
	m_Turn = ATTACKER;

	PlayGameLoop();

	delete m_Attacker;
	delete m_Defender;
}

void GameManager::PlayGameLoop()
{
	while (m_Status == PLAYING)
	{
		switch (m_Turn)
		{
		case ATTACKER:
		{
			Position attackPosition = m_Attacker->Attack();
			HitResult hitResult = m_Defender->DoHitCheck(attackPosition);

			m_Attacker->ProcessHitResult(hitResult);
			m_Board_Defender->ProcessAttack(attackPosition);
			m_Turn = DEFENDER;
			break;
		}
		case DEFENDER:
		{
			Position attackPosition = m_Defender->Attack();
			HitResult hitResult = m_Attacker->DoHitCheck(attackPosition);

			m_Defender->ProcessHitResult(hitResult);
			m_Board_Attacker->ProcessAttack(attackPosition);
			m_Turn = ATTACKER;
			break;
		}
		default:
			break;
		}
		m_Status = CheckGameStatus();
	}
}

GameStatus GameManager::CheckGameStatus()
{
	if (m_Board_Attacker->IsAllSunk() 
		|| m_Board_Defender->IsAllSunk())
		return GAMEOVER;

	return PLAYING;
}
