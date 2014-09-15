#pragma once
#include "Enums.h"

#define CHAR_X1 '1'
#define CHAR_Y1 'A'

class Network;

class Board
{
public:
	Board();
	~Board();

	void	SetBoardName(std::string name) { m_Name = name; }
	int		GetMaxWidth() { return m_Width; }
	int		GetMaxHeight() { return m_Height; }

	void	InitBoard();
	void	PrintBoard(Position pos);
	void	UpdateBoard(Position pos, Owner owner);
	void	ProcessAttack(Position pos);
	void	ProcessAttack(Position pos, HitResult info);
	void	ProcessDestroy(std::vector<Position> shipPos);
	void	AddPosition(int x, int y, ShipType shipType);
	bool	IsShipHere(int x, int y);
	bool	IsValidAttack(int x, int y);
	bool	MapCheck(int x, int y);

private:
	std::string CoordTrans(char coord);

private:
	std::string m_Name;
	int			m_Height;
	int			m_Width;
	int**		m_Board;
};

