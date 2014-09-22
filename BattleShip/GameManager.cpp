#include "stdafx.h"
#include "GameManager.h"
#include "CustomDialogBox.h"
#include "Print.h"
#include "Sound.h"
#include "Board.h"
#include "Ship.h" 
#include "Menu.h"
#include "Human.h"
#include "AI.h"


GameManager::GameManager()
{
	m_DialogBox			= nullptr;
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

/*
	������ ���η��� �Լ�
	- MainMenu���� ���� ��尡 �����Ǹ�, �̿� ���� �ٸ� ������ ������.
	- �ش� ������ ������, �ʱ�ȭ������ ���ư���.
	- m_MainLoopOn�� off�� �Ǹ� ������ ������ ����ȴ�.
*/
void GameManager::GameRun()
{
	while (m_MainLoopOn)
	{
		MainScreen();
		Menu::Instance().MainMenu();
		if (!m_MainLoopOn) break;

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

/*
	���η��� ���۽� �⺻���� ������ �ʱ�ȭ�ϰ� ���ȭ���� ����ϴ� �Լ�
*/
void GameManager::MainScreen()
{
	srand((unsigned int)time(NULL));
	system("mode con: lines=36 cols=112");

	// �Է��� ������ ������ ���ȭ���� ��� ����Ѵ�.
	while (!_kbhit())
	{
		system("cls");
		Board tempBoard;
		tempBoard.SetBoardName("<( BattleShip )>");
		tempBoard.PrintBoard({ (CONSOLE_COLS - tempBoard.GetMaxWidth() * 2) / 2,
			(CONSOLE_LINES - tempBoard.GetMaxHeight()) / 2 });

		Print::Instance().SpecialPrint();
		Print::Instance().Init();
	}
	_getch();
	Sound::Instance().StartSound();
}

/*
	���� �÷��� Ƚ���� �����ϴ� �Լ�
*/
void GameManager::SetGameLoopNum()
{
	do{
		// �׸���
		system("cls");
		Print::Instance().Gotoxy(CONSOLE_COLS / 3 + 3, CONSOLE_LINES / 3 + 1);
		Print::Instance().SetColor(Color::BOX);
		printf_s("��������������������������������");
		Print::Instance().Gotoxy(CONSOLE_COLS / 3 + 3, CONSOLE_LINES / 3 + 2);
		printf_s("�� Set Play Number (1 ~ 99999)��");
		Print::Instance().Gotoxy(CONSOLE_COLS / 3 + 3, CONSOLE_LINES / 3 + 3);
		printf_s("��������������������������������");
		Print::Instance().SetColor(Color::DEF);
		Print::Instance().Gotoxy(CONSOLE_COLS / 3 + 15, CONSOLE_LINES / 3 + 5);
		printf_s(": ");
		Sound::Instance().MenuMoveSound();

		// �߸��� �Է��� �����ϱ� ���� 6 bit ���� ���ڸ� �޾� ������ ��ȯ
		char temp[6];
		fgets(temp, 6, stdin);
		m_GameLoopNum = atoi(temp);

	} while (m_GameLoopNum < 1 || m_GameLoopNum > MAX_PLAY_NUM);

	fflush(stdin);
	Sound::Instance().MenuEnterSound();
	system("cls");
}

/*
	���� �÷��� ���� �Լ�
*/
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

				if (m_PlayerType == AI_PLAYER)
				{
					AI* temp = (AI*)m_Player1;
					temp->UpdatePriority(attackPos, hitResult);
				}

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

				AI* temp = (AI*)m_Player2;
				temp->UpdatePriority(attackPos, hitResult);

				if (m_PrintOn)
					m_DialogBox->InputSystemMessage(hitResult, PLAYER_2);

				m_Turn = PLAYER_1;
				break;
			}
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

/*
	���� �ʱ�ȭ �Լ�
*/
void GameManager::InitGame()
{
	if (m_GameMode == NETWORK_PLAY) puts("���� ����!!");
	else
	{
		m_Status = PLAYING;
		m_Turn = PLAYER_1;
	}
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

	// ��Ʈ��ũ �÷����� ��� ����� ��� ���� ��ġ�ϴ°� �ƴϹǷ� �������� �ʴ´�.
	m_Player1->SettingShips();
	if (m_GameMode == SINGLE_PLAY)
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

/*
	���� ���� �� �ʿ��� ������ �۾��� �ϴ� �Լ�
*/
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

/*
	�÷��̾� ���� �� ���� �Լ�
*/
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

/*
	�÷��̾� ���� �Լ�
	- �ڽ� Ŭ������ �Ҹ��ڸ� ȣ���ϱ� ���ؼ� �ٿ�ĳ���� �� �� delete �Ѵ�.
*/
void GameManager::DelPlayer()
{
	// 1p ����
	if (m_Player1->GetPlayerType() == AI_PLAYER)
		delete (AI*)m_Player1;
	else
		delete (Human*)m_Player1;
	// 2p ����
	if (m_Player2->GetPlayerType() == AI_PLAYER)
		delete (AI*)m_Player2;
	else
		delete (Human*)m_Player2;
}

/*
	��Ʈ��ũ ���̺귯���� ����� �¶��� ������ �ְ��ϴ� �Լ�
*/
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
		network.Connect("10.73.42.117", 9000);
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
		int number = 141033;
		error = network.SubmitName(name,number);
		if (error == ET_DUPLICATED_NAME)
		{
			puts("�̹� �����ϴ� �̸��Դϴ�.");
			return;
		}

		/*
		** ���� ���� ���
		PKT_SC_START_GAME ��Ŷ�� ��ٸ���.
		*/
		Network::GameStartData gameStartData;
		puts("���� ���� �����");
		network.WaitForStart(&gameStartData);
		wprintf_s(L"��Ī�Ǿ����ϴ�. ���� �̸�: %s, �й�: %d\n", gameStartData.oppositionName, gameStartData.oppositionStudentID);

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
			InitGame();
			/*
			** �� ����
			�ڽ��� ��ġ�� �� �����͸� ������ �����Ѵ�.
			�� �����ʹ� char�� 64ũ�� �迭�̴�.
			8*8 ���� �־��ּ���
			*/
			ShipData shipData;
			char mapData[MAP_SIZE];
			bool loop = true;
			while (loop)
			{
				shipData = TransforShipData(m_Player1);
				shipData.ToMapData(mapData);
				error = network.SubmitMap(mapData);
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
					loop = false;
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
					bool loop = true;
					while (loop)
					{
						Position attackPos = m_Player1->Attack();

						error = network.SubmitAttack(Coord(attackPos.x, attackPos.y));
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
							loop = false;
					}
					break;
				}

					// ���� ���
				case PKT_SC_ATTACK_RESULT:
				{
					Network::AttackResultData attackResult;
					attackResult = network.GetAttackResult();
					
					Position attackPos = { attackResult.pos.mX, attackResult.pos.mY };
					HitResult hitResult = TransforHitResult(attackResult.attackResult);

					
					if (attackResult.isMine)
					{
						if (m_PlayerType == AI_PLAYER)
						{
							AI* temp = (AI*)m_Player1;
							temp->UpdatePriority(attackPos, hitResult);
						}
						m_BoardPlayer2->ProcessAttack(attackPos, hitResult);
					}
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
					Network::GameResultData gameResult;
					gameResult = network.GetGameResult();
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
				Network::FinalResultData finalResult;
				finalResult = network.GetFinalResult();
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

/*
	������ ��ġ(pos)�� Ű(num)�� �޾� �ش� ������ ��ġ�� �����ϴ� �Լ�
*/
void GameManager::SetBoardPos(Position pos, int num)
{
	_ASSERT(!(num<1 || num>2 || pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES));
	if (num<1 || num>2 || pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;

	m_BoardPos[num - 1] = pos;
}

/*
	������ Ű(num)�� �޾� �ش� ������ ��ġ�� ��ȯ�ϴ� �Լ�
*/
Position GameManager::GetBoardPos(int num)
{
	_ASSERT(!(num<1 || num>2));
	if (num<1 || num>2)
		return { 0, 0 };

	return m_BoardPos[num];
}

/*
	��� �谡 ħ���ߴ��� �˻��� ������ ���������� üũ�ϴ� �Լ�
*/
GameStatus GameManager::CheckGameStatus()
{
	if (m_Player1->IsAllSunk()
		|| m_Player2->IsAllSunk()
		|| m_Status == GAMEOVER)
		return GAMEOVER;

	return PLAYING;
}

/*
	��ġ ��ǥ ������ ��Ʈ��ũ ������ ���� �ڷ������� ��ȯ�ϴ� �Լ�
*/
ShipData GameManager::TransforShipData(Player* player)
{
	ShipData ret;

	_ASSERT(!(player == nullptr));
	if (player == nullptr)
		return ret;

	Coord temp[MAX_SHIP_NUM + 1][MAX_SHIP_LEN];
	for (int idx = 1; idx <= MAX_SHIP_NUM; ++idx)
	{
		auto ship = player->GetShipList()->at(idx - 1);
		for (int l = 0; l < ship->GetMaxHP(); l++)
		{
			Position pos = ship->GetShipPos()[l];
			temp[idx][l] = Coord(pos.x, pos.y);
		}
		ret.SetShip((ShipData::ShipType)(idx), temp[idx]);
	}
	return ret;
}

/*
	��Ʈ��ũ���� ��Ŷ���� ���޹��� ���ݰ�� ������ �� ���α׷��� �´� �ڷ������� ��ȯ�ϴ� �Լ�
*/
HitResult GameManager::TransforHitResult(short info)
{
	switch (info)
	{
	case AR_MISS:
		return MISS;
	case AR_HIT:
		return HIT;
	case AR_DESTROY_AIRCRAFT:
		return DESTROY_AIRCRAFT;
	case AR_DESTROY_BATTLESHIP:
		return DESTROY_BATTLESHIP;
	case AR_DESTROY_CRUISER:
		return DESTROY_CRUISER;
	case AR_DESTROY_DESTROYER:
		return DESTROY_DESTROYER;
	default:
		return HitResult(NO_RESULT);
	}
}
