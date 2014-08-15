#pragma once
#include "Position.h"

#define HIT_FLAG_MULTIPLIER 10

class Board
{
public:
	Board();
	~Board();

	void SetBoardName(std::string name) { m_Name = name; }
	int GetMaxWidth() { return m_Width; }
	int GetMaxHeight() { return m_Height; }

	//void DrawBoard();
	void AddPosition(int x, int y, int value);
	void PrintBoard();
	void ProcessAttack(Position pos);
	bool DuplCheck(int x, int y);
	bool MapCheck(int posX, int posY);
	bool IsShipHere(int x, int y);

private:
	std::string m_Name;
	int m_Width;
	int m_Height;
	int** m_Board;
};

