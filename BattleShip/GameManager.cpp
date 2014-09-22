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
	게임의 메인루프 함수
	- MainMenu에서 게임 모드가 결정되며, 이에 따라 다른 로직을 따른다.
	- 해당 게임이 끝나면, 초기화면으로 돌아간다.
	- m_MainLoopOn이 off가 되면 게임이 완전히 종료된다.
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
	메인루프 시작시 기본적인 설정을 초기화하고 대기화면을 출력하는 함수
*/
void GameManager::MainScreen()
{
	srand((unsigned int)time(NULL));
	system("mode con: lines=36 cols=112");

	// 입력이 들어오기 전까지 대기화면을 계속 재생한다.
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
	게임 플레이 횟수를 지정하는 함수
*/
void GameManager::SetGameLoopNum()
{
	do{
		// 그리기
		system("cls");
		Print::Instance().Gotoxy(CONSOLE_COLS / 3 + 3, CONSOLE_LINES / 3 + 1);
		Print::Instance().SetColor(Color::BOX);
		printf_s("┌──────────────┐");
		Print::Instance().Gotoxy(CONSOLE_COLS / 3 + 3, CONSOLE_LINES / 3 + 2);
		printf_s("│ Set Play Number (1 ~ 99999)│");
		Print::Instance().Gotoxy(CONSOLE_COLS / 3 + 3, CONSOLE_LINES / 3 + 3);
		printf_s("└──────────────┘");
		Print::Instance().SetColor(Color::DEF);
		Print::Instance().Gotoxy(CONSOLE_COLS / 3 + 15, CONSOLE_LINES / 3 + 5);
		printf_s(": ");
		Sound::Instance().MenuMoveSound();

		// 잘못된 입력을 방지하기 위해 6 bit 내의 문자만 받아 정수로 변환
		char temp[6];
		fgets(temp, 6, stdin);
		m_GameLoopNum = atoi(temp);

	} while (m_GameLoopNum < 1 || m_GameLoopNum > MAX_PLAY_NUM);

	fflush(stdin);
	Sound::Instance().MenuEnterSound();
	system("cls");
}

/*
	게임 플레이 루프 함수
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
	게임 초기화 함수
*/
void GameManager::InitGame()
{
	if (m_GameMode == NETWORK_PLAY) puts("게임 시작!!");
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

	// 네트워크 플레이의 경우 상대편 배는 내가 배치하는게 아니므로 셋팅하지 않는다.
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
	게임 종료 전 필요한 마무리 작업을 하는 함수
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
	플레이어 생성 및 설정 함수
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
	플레이어 제거 함수
	- 자식 클래스의 소멸자를 호출하기 위해서 다운캐스팅 한 후 delete 한다.
*/
void GameManager::DelPlayer()
{
	// 1p 제거
	if (m_Player1->GetPlayerType() == AI_PLAYER)
		delete (AI*)m_Player1;
	else
		delete (Human*)m_Player1;
	// 2p 제거
	if (m_Player2->GetPlayerType() == AI_PLAYER)
		delete (AI*)m_Player2;
	else
		delete (Human*)m_Player2;
}

/*
	네트워크 라이브러리를 사용한 온라인 대전을 주관하는 함수
*/
void GameManager::NetworkManager()
{
	Network network;
	PacketType type;
	ErrorType error;

	// 네트워크 초기화
	try
	{
		Network::Initialize();
	}
	catch (Network::Exception)
	{
		puts("초기화 도중 문제가 발생했습니다.");
		return;
	}

	// 서버에 연결
	try
	{
		network.Connect("10.73.42.117", 9000);
	}
	catch (Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("서버와 연결에 실패했습니다.");
			break;
		case Network::PARAMETER_ERROR:
			puts("함수의 인수가 잘못되었습니다.");
			break;
		}
		return;
	}
	puts("접속 성공!");

	// 예외 처리를 위해 try문으로 모두 감싼다.
	try	
	{
		/*
		** 이름 전송
		최대 길이는 MAX_NAME_LEN-1 == 15글자.
		성공시 ET_OK가 리턴된다.
		이미 있는 이름을 쓰면 ET_DUPLICATED_NAME이 온다.
		*/
		const wchar_t name[MAX_NAME_LEN] = L"잉여";
		int number = 141033;
		error = network.SubmitName(name,number);
		if (error == ET_DUPLICATED_NAME)
		{
			puts("이미 존재하는 이름입니다.");
			return;
		}

		/*
		** 게임 시작 대기
		PKT_SC_START_GAME 패킷을 기다린다.
		*/
		Network::GameStartData gameStartData;
		puts("게임 시작 대기중");
		network.WaitForStart(&gameStartData);
		wprintf_s(L"매칭되었습니다. 상대방 이름: %s, 학번: %d\n", gameStartData.oppositionName, gameStartData.oppositionStudentID);

		/*
		** 게임 시작
		맵 제출부터 게임 종료까지 n회 반복한다.
		하나의 게임이 끝나고 다음 게임을 시작해야 한다면 PKT_SC_NEXT_GAME 패킷이
		모든 게임이 끝나면 PKT_SC_ALL_OVER 패킷이 들어온다.
		*/
		SetPlayer();
		bool allOver = false;
		while (!allOver)
		{
			InitGame();
			/*
			** 맵 제출
			자신이 배치한 맵 데이터를 서버로 전송한다.
			맵 데이터는 char형 64크기 배열이다.
			8*8 맵을 넣어주세욜
			*/
			ShipData shipData;
			char mapData[MAP_SIZE];
			while (true)
			{
				shipData = TransforShipData(m_Player1);
				shipData.ToMapData(mapData);
				error = network.SubmitMap(mapData);
				if (error == ET_INVALID_MAP)
				{
					if (m_PrintOn)
					{
						m_DialogBox->InputSystemMessage("<< 유효하지 않은 맵 데이터입니다.");
						m_DialogBox->InitDialog();
						m_DialogBox->PrintDialog();
					}
				}
				else
					break;
			}

			/*
			** 게임 루프
			내 차례라면 공격 위치를 전송한다.
			차례가 끝나면 공격자와 공격 위치, 공격 결과를 받는다.
			한 게임이 끝나면 PKT_SC_GAME_OVER 패킷이 들어온다.
			*/
			bool gameOver = false;
			while (!gameOver)
			{
				error = network.GetPacketType(&type);

				switch (type)
				{
					// 에러가 발생하는 경우(상대방의 접속 종료)
				case PKT_SC_ERROR:
					if (error == ET_OPPOSITION_DISCONNECTED)
					{
						if (m_PrintOn)
						{
							m_DialogBox->InputSystemMessage("<< 상대방의 접속이 끊어졌습니다.");
							m_DialogBox->InitDialog();
							m_DialogBox->PrintDialog();
						}
					}
					else
					{
						if (m_PrintOn)
						{
							m_DialogBox->InputSystemMessage("<< 알 수 없는 에러입니다.");
							m_DialogBox->InitDialog();
							m_DialogBox->PrintDialog();
						}
					}
					return;

					// 내 차례
				case PKT_SC_MY_TURN:
				{
					/*
					** 공격 위치 전송
					x, y는 0~7 사이의 정수이다.
					*/
					while (true)
					{
						Position attackPos = m_Player1->Attack();

						error = network.SubmitAttack(Coord(attackPos.x, attackPos.y));
						if (error == ET_INVALID_ATTACK)
						{
							if (m_PrintOn)
							{
								m_DialogBox->InputSystemMessage("<< 유효하지 않은 공격 위치입니다.");
								m_DialogBox->InitDialog();
								m_DialogBox->PrintDialog();
							}
						}
						else
							break;
					}
					break;
				}

					// 공격 결과
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

					// 게임 종료
				case PKT_SC_GAME_OVER:
				{
					Network::GameResultData gameResult;
					gameResult = network.GetGameResult();
					if (gameResult.isWinner)
					{
						if (m_PrintOn)
						{
							m_DialogBox->InputSystemMessage(">> 승리!!!");
							m_DialogBox->InitDialog();
							m_DialogBox->PrintDialog();
						}
						else puts("승리!!!");
						Sound::Instance().CloseSound();
					}
					else
					{
						if (m_PrintOn)
						{
							m_DialogBox->InputSystemMessage(">> 패배...");
							m_DialogBox->InitDialog();
							m_DialogBox->PrintDialog();
						}
						else puts("패배...");
					}
					Print::Instance().Gotoxy(0, 0);
					printf_s("턴 수: %d\n", gameResult.turns);
					gameOver = true;
					break;
				}

				default:
					throw Network::UNEXPECTED_PACKET;
					break;
				}
			}

			/*
			** 종료후 처리
			*/
			system("cls");
			network.GetPacketType(&type);

			if (type == PKT_SC_NEXT_GAME)
			{
				puts("다음 게임을 준비해주세요.");
			}
			else if (type == PKT_SC_ALL_OVER)
			{
				Network::FinalResultData finalResult;
				finalResult = network.GetFinalResult();
				puts("모두 종료");
				printf_s("승리 횟수: %d, 평균 턴 수: %.1f", finalResult.winCount, finalResult.avgTurns);

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
			puts("네트워크에 문제가 발생했습니다.");
			break;
		case Network::SERVER_CLOSED:
			puts("서버와의 연결이 끊어졌습니다.");
			break;
		case Network::PARAMETER_ERROR:
			puts("함수의 인수가 잘못되었습니다.");
			break;
		case Network::UNEXPECTED_PACKET:
			puts("서버에서 잘못된 정보가 전송되었습니다.");
			break;
		default:
			break;
		}
	}

	/*
	** 연결 종료
	참고로 소멸시에도 자동으로 Disconnect를 호출한다.
	*/
	network.Disconnect();
}

/*
	보드의 위치(pos)와 키(num)를 받아 해당 보드의 위치를 지정하는 함수
*/
void GameManager::SetBoardPos(Position pos, int num)
{
	if (num<1 || num>2 || pos.x<0 || pos.x>CONSOLE_COLS || pos.y<0 || pos.y>CONSOLE_LINES)
		return;
	m_BoardPos[num - 1] = pos;
}

/*
	보드의 키(num)를 받아 해당 보드의 위치를 반환하는 함수
*/
Position GameManager::GetBoardPos(int num)
{
	if (num<1 || num>2)
		return { 0, 0 };
	return m_BoardPos[num];
}

/*
	모든 배가 침몰했는지 검사해 게임이 끝났는지를 체크하는 함수
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
	배치 좌표 정보를 네트워크 전송을 위한 자료형으로 변환하는 함수
*/
ShipData GameManager::TransforShipData(Player* player)
{
	ShipData ret;
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
	네트워크에서 패킷으로 전달받은 공격결과 정보를 이 프로그램에 맞는 자료형으로 변환하는 함수
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
