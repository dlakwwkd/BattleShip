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
	m_DialogBox		= nullptr;
	m_Player1			= nullptr;
	m_Player2			= nullptr;
	m_BoardPlayer1		= nullptr;
	m_BoardPlayer2		= nullptr;
	m_PlayerType		= AI_PLAYER;
	m_Status			= GAMEOVER;
	m_GameMode			= SINGLE_PLAY;
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
		if (!m_MainLoopOn) return;

		switch (m_GameMode)
		{
		case SINGLE_PLAY:
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
			break;
		}
		case NETWORK_PLAY:
			NetworkManager();
			CloseGame();
			break;
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
					m_DialogBox->InitDialog();
					m_DialogBox->PrintDialog();
				}
				Position attackPos = m_Player1->Attack();
				m_BoardPlayer2->ProcessAttack(attackPos);
				HitResult hitResult = m_Player2->DoHitCheck(attackPos);
// 				if (m_Player1->GetPlayerType() == AI_PLAYER)
// 				{
// 					
// 				}


				if (m_PrintOn)
					m_DialogBox->InputSystemMessage(hitResult, PLAYER_1);

				m_Turn = PLAYER_2;
				break;
			}
		case PLAYER_2:
			{
				Position attackPos = m_Player2->Attack();
				m_BoardPlayer1->ProcessAttack(attackPos);
				HitResult hitResult = m_Player1->DoHitCheck(attackPos);

				if (m_PrintOn)
					m_DialogBox->InputSystemMessage(hitResult, PLAYER_2);

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
			m_DialogBox->InitDialog();
			m_DialogBox->PrintDialog();
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
	if (m_DialogBox) delete m_DialogBox;
	if (m_PrintOn)
	{
		Print::Instance().Init();
		Sound::Instance().StartSound();
		m_DialogBox = new CustomDialogBox();
		if (m_PlayerType == HUMAN_PLAYER)
		{
			Human* temp = (Human*)m_Player1;
			temp->SetMyDialogBox(m_DialogBox);
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
		m_DialogBox->InitDialog();
		m_DialogBox->PrintDialog();
	}
}

void GameManager::CloseGame()
{
	if (m_PrintOn)
		Print::Instance().PrintText();
	Print::Instance().Init();
	if (m_DialogBox) delete m_DialogBox;
	m_DialogBox = nullptr;
	m_EachGameTurnNum = 0;
	m_TotalGameTurnNum = 0;
	fflush(stdin);
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


void GameManager::NetworkManager()
{

	Network network;
	PacketType type;
	ErrorType error;

	// ��Ʈ��ũ �ʱ�ȭ
	try
	{
		Network::Initialize();
	}
	catch (Network::Exception)
	{
		puts("�ʱ�ȭ ���� ������ �߻��߽��ϴ�.");
		return;
	}

	// ������ ����
	try
	{
		network.Connect("10.73.42.117", 9001);
	}
	catch (Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("������ ���ῡ �����߽��ϴ�.");
			break;
		case Network::PARAMETER_ERROR:
			puts("�Լ��� �μ��� �߸��Ǿ����ϴ�.");
			break;
		}
		return;
	}
	puts("���� ����!");

	// ���� ó���� ���� try������ ��� ���Ѵ�.
	try	
	{
		/*
		** �̸� ����
		�ִ� ���̴� MAX_NAME_LEN-1 == 15����.
		������ ET_OK�� ���ϵȴ�.
		�̹� �ִ� �̸��� ���� ET_DUPLICATED_NAME�� �´�.
		*/
		const wchar_t name[MAX_NAME_LEN] = L"�׿�";

		error = network.SubmitName(name);
		if (error == ET_DUPLICATED_NAME)
		{
			puts("�̹� �����ϴ� �̸��Դϴ�.");
			return;
		}

		/*
		** ���� ���� ���
		PKT_SC_START_GAME ��Ŷ�� ��ٸ���.
		*/
		puts("���� ���� �����..");
		network.WaitSpecPacket(PKT_SC_GAME_START);

		/*
		** ���� ����
		�� ������� ���� ������� nȸ �ݺ��Ѵ�.
		�ϳ��� ������ ������ ���� ������ �����ؾ� �Ѵٸ� PKT_SC_NEXT_GAME ��Ŷ��
		��� ������ ������ PKT_SC_ALL_OVER ��Ŷ�� ���´�.
		*/
		SetPlayer();
		bool allOver = false;
		while (!allOver)
		{
			InitNetworkGame();
			/*
			** �� ����
			�ڽ��� ��ġ�� �� �����͸� ������ �����Ѵ�.
			�� �����ʹ� char�� 32ũ�� �迭�̴�.
			Aircraft���� ������� ���� ��ǥ���� �ִ´�.
			*/
			Network::MapData mapData;
			while (true)
			{
				mapData = TransforMapData(m_Player1);

				error = network.SubmitMap(&mapData);
				if (error == ET_INVALID_MAP)
				{
					if (m_PrintOn)
					{
						m_DialogBox->InputSystemMessage("<< ��ȿ���� ���� �� �������Դϴ�.");
						m_DialogBox->InitDialog();
						m_DialogBox->PrintDialog();
					}
				}
				else
					break;
			}

			/*
			** ���� ����
			�� ���ʶ�� ���� ��ġ�� �����Ѵ�.
			���ʰ� ������ �����ڿ� ���� ��ġ, ���� ����� �޴´�.
			�� ������ ������ PKT_SC_GAME_OVER ��Ŷ�� ���´�.
			*/
			bool gameOver = false;
			while (!gameOver)
			{
				error = network.GetPacketType(&type);

				switch (type)
				{
					// ������ �߻��ϴ� ���(������ ���� ����)
				case PKT_SC_ERROR:
					if (error == ET_OPPOSITION_DISCONNECTED)
					{
						if (m_PrintOn)
						{
							m_DialogBox->InputSystemMessage("<< ������ ������ ���������ϴ�.");
							m_DialogBox->InitDialog();
							m_DialogBox->PrintDialog();
						}
					}
					else
					{
						if (m_PrintOn)
						{
							m_DialogBox->InputSystemMessage("<< �� �� ���� �����Դϴ�.");
							m_DialogBox->InitDialog();
							m_DialogBox->PrintDialog();
						}
					}
					return;

					// �� ����
				case PKT_SC_MY_TURN:
				{
					/*
					** ���� ��ġ ����
					x, y�� 0~7 ������ �����̴�.
					*/
					while (true)
					{
						Position attackPos = m_Player1->Attack();

						error = network.SubmitAttack(attackPos.x, attackPos.y);
						if (error == ET_INVALID_ATTACK)
						{
							if (m_PrintOn)
							{
								m_DialogBox->InputSystemMessage("<< ��ȿ���� ���� ���� ��ġ�Դϴ�.");
								m_DialogBox->InitDialog();
								m_DialogBox->PrintDialog();
							}
						}
						else
							break;
					}
					break;
				}

					// ���� ���
				case PKT_SC_ATTACK_RESULT:
				{
					Network::AttackResult attackResult;
					network.GetAttackResult(&attackResult);

					Position attackPos = { attackResult.x, attackResult.y };
					HitResult hitResult = TransforHitResult(attackResult.attackResult);

					if (attackResult.isMine)
						m_BoardPlayer2->ProcessAttack(attackPos, hitResult);
					else
						m_BoardPlayer1->ProcessAttack(attackPos);

					if (m_PrintOn)
					{
						m_Player1->GetMyBoard()->UpdateBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y }, IAM);
						m_Player1->GetEnemyBoard()->UpdateBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y }, ENEMY);
						Print::Instance().PrintText();
						Print::Instance().Init();
						m_DialogBox->InputSystemMessage(hitResult, PLAYER_1);
						m_DialogBox->InitDialog();
						m_DialogBox->PrintDialog();
					}
					break;
				}

					// ���� ����
				case PKT_SC_GAME_OVER:
				{
					Network::GameResult gameResult;
					network.GetGameResult(&gameResult);
					if (gameResult.isWinner)
					{
						if (m_PrintOn)
						{
							m_DialogBox->InputSystemMessage(">> �¸�!!!");
							m_DialogBox->InitDialog();
							m_DialogBox->PrintDialog();
						}
						else puts("�¸�!!!");
						Sound::Instance().CloseSound();
					}
					else
					{
						if (m_PrintOn)
						{
							m_DialogBox->InputSystemMessage(">> �й�...");
							m_DialogBox->InitDialog();
							m_DialogBox->PrintDialog();
						}
						else puts("�й�...");
					}
					Print::Instance().Gotoxy(0, 0);
					printf_s("�� ��: %d\n", gameResult.turns);
					gameOver = true;
					break;
				}

				default:
					throw Network::UNEXPECTED_PACKET;
					break;
				}
			}

			/*
			** ������ ó��
			*/
			system("cls");
			network.GetPacketType(&type);

			if (type == PKT_SC_NEXT_GAME)
			{
				puts("���� ������ �غ����ּ���.");
			}
			else if (type == PKT_SC_ALL_OVER)
			{
				Network::FinalResult finalResult;
				network.GetFinalResult(&finalResult);
				puts("��� ����");
				printf_s("�¸� Ƚ��: %d, ��� �� ��: %.1f", finalResult.winCount, finalResult.avgTurns);

				allOver = true;
			}
			else
				throw Network::UNEXPECTED_PACKET;
		}
		DelPlayer();
	}
	catch (Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("��Ʈ��ũ�� ������ �߻��߽��ϴ�.");
			break;
		case Network::SERVER_CLOSED:
			puts("�������� ������ ���������ϴ�.");
			break;
		case Network::PARAMETER_ERROR:
			puts("�Լ��� �μ��� �߸��Ǿ����ϴ�.");
			break;
		case Network::UNEXPECTED_PACKET:
			puts("�������� �߸��� ������ ���۵Ǿ����ϴ�.");
			break;
		default:
			break;
		}
	}


	/*
	** ���� ����
	����� �Ҹ�ÿ��� �ڵ����� Disconnect�� ȣ���Ѵ�.
	*/
	network.Disconnect();
}

void GameManager::InitNetworkGame()
{
	if (m_GameMode == NETWORK_PLAY) puts("���� ����!!");
	m_Player1->InitPlayer();
	m_Player2->InitPlayer();
	if (m_DialogBox) delete m_DialogBox;
	if (m_PrintOn)
	{
		Print::Instance().Init();
		Sound::Instance().StartSound();
		m_DialogBox = new CustomDialogBox();
		if (m_PlayerType == HUMAN_PLAYER)
		{
			Human* temp = (Human*)m_Player1;
			temp->SetMyDialogBox(m_DialogBox);
		}
	}
	m_BoardPlayer1 = m_Player1->GetMyBoard();
	m_BoardPlayer2 = m_Player2->GetMyBoard();
	m_Player1->SetEnemyBoard(m_BoardPlayer2);
	m_Player1->GetMyBoard()->PrintBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y });
	m_Player1->GetEnemyBoard()->PrintBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y });

	m_Player1->SettingShips();

	m_Player1->GetMyBoard()->UpdateBoard({ MY_BOARD_POS_X, MY_BOARD_POS_Y }, IAM);
	m_Player1->GetEnemyBoard()->UpdateBoard({ ENEMY_BOARD_POS_X, ENEMY_BOARD_POS_Y }, ENEMY);

	if (m_PrintOn)
	{
		Print::Instance().PrintText();
		Print::Instance().Init();
		m_DialogBox->InitDialog();
		m_DialogBox->PrintDialog();
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

Network::MapData GameManager::TransforMapData(Player* player)
{
	Network::MapData temp;
	int i = 0;
	for (auto& coord : temp.aircraft)
	{
		Position pos = player->GetShipList()->at(0)->GetShipPos()[i++];
		coord = { pos.x, pos.y };
	}
	i = 0;
	for (auto& coord : temp.battleship)
	{
		Position pos = player->GetShipList()->at(1)->GetShipPos()[i++];
		coord = { pos.x, pos.y };
	}
	i = 0;
	for (auto& coord : temp.cruiser)
	{
		Position pos = player->GetShipList()->at(2)->GetShipPos()[i++];
		coord = { pos.x, pos.y };
	}
	i = 0;
	for (auto& coord : temp.destroyer1)
	{
		Position pos = player->GetShipList()->at(3)->GetShipPos()[i++];
		coord = { pos.x, pos.y };
	}
	i = 0;
	for (auto& coord : temp.destroyer2)
	{
		Position pos = player->GetShipList()->at(4)->GetShipPos()[i++];
		coord = { pos.x, pos.y };
	}
	return temp;
}

HitResult GameManager::TransforHitResult(short info)
{
	switch (info)
	{
	case AR_MISS:
		return MISS;
	case AR_HIT:
		return HIT;
	case AR_DESTORY_AIRCRAFT:
		return DESTROY_AIRCRAFT;
	case AR_DESTORY_BATTLESHIP:
		return DESTROY_BATTLESHIP;
	case AR_DESTORY_CRUISER:
		return DESTROY_CRUISER;
	case AR_DESTORY_DESTORYER:
		return DESTROY_DESTROYER;
	default:
		return HitResult(NO_RESULT);
	}
}
