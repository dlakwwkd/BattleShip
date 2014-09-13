#pragma once
#include "Setting.h"
#include "Enums.h"

class Board
{
public:
	Board();
	~Board();

	void	SetBoardName(std::string name) { m_Name = name; }
	int		GetMaxWidth() { return m_Width; }
	int		GetMaxHeight() { return m_Height; }

	void	InitBoard();
	void	PrintBoard(POINT pos);
	void	UpdateBoard(POINT pos, Owner owner);
	void	ProcessAttack(Position pos);
	void	ProcessHitResult(HitResult hit);
	void	AddPosition(int x, int y, ShipType shipType);
	bool	IsShipHere(int x, int y);
	bool	IsValidAttack(int x, int y);
	bool	MapCheck(int x, int y);

private:
	std::string CoordTrans(char coord);

private:
	std::string m_Name;
	int			m_Width;
	int			m_Height;
	int**		m_Board;
};

