#pragma once
#include "Enums.h"

class CustomDialogBox;
class Player;
class Board;
struct Position;

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

	void		PlayGameLoop();
	void		InitGame();
	void		CloseGame();
	void		SetPlayer();
	void		DelPlayer();
	void		SetBoardPos(POINT pos, int num);
	POINT		GetBoardPos(int num);
	GameStatus	CheckGameStatus();

private:
	CustomDialogBox*	m_DialogBox;
	Player*				m_Player1;
	Player*				m_Player2;
	Board*				m_Board_Player1;
	Board*				m_Board_Player2;
	GameStatus			m_Status;
	Turn				m_Turn;
	POINT				m_Board_Pos[2];
};

