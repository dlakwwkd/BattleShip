#pragma once
class CustomDialogBox;
class Player;
class Board;

#define MAX_PLAY_NUM 99999

class GameManager
{
public:
	static GameManager& Instance()
	{
		static GameManager instance;
		return instance;
	}
	void		GameRun();
	void		GameOver() { m_Status = GAMEOVER; }
	void		SetGameMode(GameMode mode){ m_GameMode = mode; }
	void		SetGameOnOff(OnOff swit) { m_MainLoopOn = swit; }
	void		SetPrintOnOff(OnOff swit) { m_PrintOn = swit; }
	void		SetPlayerType(PlayerType type) { m_PlayerType = type; }
	OnOff		GetMainLoopStatus() { return m_MainLoopOn; }

private:
	GameManager();

	void				MainScreen();
	void				SetGameLoopNum();
	void				PlayGameLoop();
	void				InitGame();
	void				CloseGame();
	void				SetPlayer();
	void				DelPlayer();
	void				NetworkManager();
	void				SetBoardPos(Position pos, int num);
	Position			GetBoardPos(int num);
	GameStatus			CheckGameStatus();
	ShipData			TransforShipData(Player* player);
	HitResult			TransforHitResult(short info);

private:
	CustomDialogBox*	m_DialogBox;
	Player*				m_Player1;
	Player*				m_Player2;
	Board*				m_BoardPlayer1;
	Board*				m_BoardPlayer2;
	PlayerType			m_PlayerType;
	GameStatus			m_Status;
	GameMode			m_GameMode;
	Turn				m_Turn;
	Position			m_BoardPos[2];
	OnOff				m_MainLoopOn;
	OnOff				m_PrintOn;
	int					m_GameLoopNum;
	int					m_EachGameTurnNum;
	int					m_TotalGameTurnNum;
};

