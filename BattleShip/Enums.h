#pragma once
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

enum HitResult
{
	HIT,
	MISS,
	DESTROY,

	DESTROY_AIRCRAFT,
	DESTROY_BATTLESHIP,
	DESTROY_CRUISER,
	DESTROY_DESTROYER,
};

enum ShipType
{
	DESTROYER	= 2,
	CRUISER		= 3,
	BATTLESHIP	= 4,
	AIRCRAFT	= 5,
};

enum GameStatus
{
	PLAYING,
	GAMEOVER,
};

enum Turn
{
	DONE,
	PLAYER_1,
	PLAYER_2,
};

enum Owner
{
	IAM,
	ENEMY,
};

enum Color
{
	DEF			= 15,
	GREEN		= 10,
	GRAY		= 8,
	SKY			= 11,
	BOX			= 63,
	DECO		= 159,
	COORD_BOX	= 185,
	MARK_BOX	= 223,
	HIT_POS		= 251,
	TILE_1		= 116,
	TILE_2		= 132,
	A_LIVE		= 196,
	B_LIVE		= 151,
	C_LIVE		= 163,
	D_LIVE		= 226,
	A_DEATH		= 71,
	B_DEATH		= 23,
	C_DEATH		= 39,
	D_DEATH		= 111,
};