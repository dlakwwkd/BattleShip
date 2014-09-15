#include "stdafx.h"
#include "Sound.h"


Sound::Sound()
{
}


void Sound::StartSound()
{
	Beep(660, 100);
	Beep(784, 100);
	Beep(784, 100);
	Beep(880, 100);
	Beep(982, 100);
}

void Sound::CloseSound()
{
	Beep(660, 200);
	Beep(660, 100);
	Beep(660, 100);
	Beep(660, 600);
	Beep(784, 300);
	Beep(660, 300);
	Beep(784, 400);
	Beep(880, 900);
	Sleep(200);
// 	Beep(784, 800);
// 	Beep(880, 800);
// 	Beep(999, 999);

}

void Sound::MissSound()
{
	//Beep(540, 200);
}

void Sound::HitSound()
{
 	Beep(540, 100);
 	Beep(780, 100);
}

void Sound::DestroySound()
{
	Beep(784, 100);
	Beep(698, 100);
	Beep(588, 100);
	Beep(698, 100);
	Beep(784, 100);
}

void Sound::MenuMoveSound()
{
 	Beep(780, 100);
}

void Sound::MenuEnterSound()
{
	Beep(660, 100);
	Beep(880, 100);
}

void Sound::MenuESCSound()
{
	Beep(880, 100);
	Beep(660, 100);
}
