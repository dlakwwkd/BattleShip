#pragma once


enum GameStatus
{
	STARTED,
	FINISHED,
};

enum HitResult
{
	MISS,
	HIT,
	DESTROYED_DESTROYER,
	DESTROYED_CRUISER,
	DESTROYED_BATTLESHIP,
	DESTROYED_AIRCRAFT,
};

enum ShipType
{
	DESTROYER	= 2,
	CRUISER		= 3,
	BATTLESHIP	= 4,
	AIRCRAFT	= 5,
};

enum PlayerType
{
	HUMAN,
	COMPUTER,
	ONLINE,
};

enum Direction
{
	DOWNWARD,
	RIGHT,
};

