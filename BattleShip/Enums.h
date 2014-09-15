#pragma once
enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
};

enum HitResult
{
	HIT = -10,
	MISS = -1,
	NO_RESULT,
	DESTROY,

	DESTROY_AIRCRAFT	= -15,
	DESTROY_BATTLESHIP	= -14,
	DESTROY_CRUISER		= -13,
	DESTROY_DESTROYER	= -12,
};

enum ShipType
{
	AIRCRAFT	= 5,
	BATTLESHIP	= 4,
	CRUISER		= 3,
	DESTROYER	= 2,
	NONE_SHIP	= 0,
};

enum GameStatus
{
	PLAYING,
	GAMEOVER,
};

enum GameMode
{
	SINGLE_PLAY,
	NETWORK_PLAY,
};

enum OnOff
{
	OFF,
	ON
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

enum PlayerType
{
	AI_PLAYER,
	HUMAN_PLAYER,
};

enum Color
{
	DEF			= 15,
	GREEN		= 10,
	RED			= 12,
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

enum InputKey
{
	SCAN_CODE	= 224,
	ENTER_KEY	= 13,
	SPACE_KEY	= 32,
	ESC_KEY		= 27,
	UP_KEY		= 72,
	DOWN_KEY	= 80,
	LEFT_KEY	= 75,
	RIGHT_KEY	= 77,
};

enum MenuType
{
	MAIN_MENU,
	PlAYER_MENU,
};
