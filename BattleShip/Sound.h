#pragma once
class Sound
{
public:
	static Sound& Instance()
	{
		static Sound instance;
		return instance;
	}
	void StartSound();
	void CloseSound();
	void MissSound();
	void HitSound();
	void DestroySound();
	void MenuMoveSound();
	void MenuEnterSound();
	void MenuESCSound();
private:
	Sound();
};

