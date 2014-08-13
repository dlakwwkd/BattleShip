#pragma once
#include "Enums.h"

class Player;
class Board;
struct Position;

class GameManager
{
public:
	GameManager();
	~GameManager();

	void StartGame();
	void TurnChange();
	int WhoWinner();
	int GetNowTurn();
private:
	void PlayGameLoop();
	void DeleteShip();
	GameStatus CheckGameStatus();

private:
	Board* m_Board_Attacker;
	Board* m_Board_Defender;
	Player* m_Attacker;
	Player* m_Defender;
	GameStatus m_Status;
	Turn m_Turn;
};

