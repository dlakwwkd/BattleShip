#pragma once
#include "Enums.h"

class CustomDialogBox;
class Player;
class Board;

class GameManager
{
public:
	static GameManager& Instance()
	{
		static GameManager instance;
		return instance;
	}
	void		GameRun();
	void		GameOff() { m_Status = GAMEOVER; }
private:
	GameManager();

	void		MainScreen();
	void		MainMenu();
	void		PlayGameLoop();
	void		InitGame();
	void		CloseGame();
	void		SetPlayer();
	void		DelPlayer();
	void		SetBoardPos(POINT pos, int num);
	POINT		GetBoardPos(int num);
	GameStatus	CheckGameStatus();

private:
	CustomDialogBox*	m_DialogBox1;
	Player*				m_Player1;
	Player*				m_Player2;
	Board*				m_BoardPlayer1;
	Board*				m_BoardPlayer2;
	GameStatus			m_Status;
	Turn				m_Turn;
	POINT				m_BoardPos[2];
	bool				m_MainLoopOn;
	int					m_GameLoopNum;
	int					m_EachGameTurnNum;
	int					m_TotalGameTurnNum;
};

