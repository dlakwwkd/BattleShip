#pragma once
#define CHAR_X1 '1'
#define CHAR_Y1 'A'

#define MY_BOARD_POS_X		32
#define MY_BOARD_POS_Y		10
#define ENEMY_BOARD_POS_X	64
#define ENEMY_BOARD_POS_Y	10

class Board
{
public:
	Board();
	~Board();

	void	SetBoardName(std::string name) { m_Name = name; }
	int		GetMaxWidth() { return m_Width; }
	int		GetMaxHeight() { return m_Height; }
	int**	GetBoard(){ return m_Board; }

	void				InitBoard();
	void				PrintBoard(Position pos);
	void				UpdateBoard(Position pos, Owner owner);
	void				ProcessAttack(Position pos);
	void				ProcessAttack(Position pos, HitResult info);
	void				ProcessDestroy(std::vector<Position> shipPos);
	void				AddPosition(Position pos, ShipType shipType);
	bool				IsShipHere(int x, int y);
	bool				IsValidAttack(int x, int y);
	bool				IsInBoard(int x, int y);
	PriorityCalcRate	IsConfinedPos(int x, int y);

private:
	std::string			CoordTrans(char coord);

private:
	std::string	m_Name;
	int			m_Height;
	int			m_Width;
	int**		m_Board;
};

