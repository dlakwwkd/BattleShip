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
	DESTROYER = 2,
	CRUISER = 3,
	BATTLESHIP = 4,
	AIRCRAFT = 5,
};

enum GameStatus
{
	PLAYING,
	GAMEOVER,
};

enum Turn
{
	ATTACKER,
	DEFENDER,
};

