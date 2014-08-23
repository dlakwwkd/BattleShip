#include "stdafx.h"
#include "Sound.h"


Sound::Sound()
{
}


void Sound::StartSound()
{
	Beep(540, 200);
	Beep(588, 200);
	Beep(660, 200);
	Beep(698, 200);
	Beep(784, 200);
	Beep(880, 200);
	Beep(963, 200);
	Beep(1048, 500);
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
	Beep(784, 800);
	Beep(880, 800);
	Beep(999, 900);

}

void Sound::MissSound()
{
	Beep(349, 200);
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
