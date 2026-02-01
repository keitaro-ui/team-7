#pragma once

#include "System/AudioSource.h"

enum SoundList
{
	titleBGM,
	gameBGM,
	resultOverBGM,
	/*resultClearBGM,
	walkSE,
	numSE,
	flagSE,
	startSE,
	heartSE,*/
	SlideSE,
	playerSE,


	SoundCount
};

class SoundManager
{
private:
	SoundManager() {}
	~SoundManager() {}

public:
	static SoundManager& Instance()
	{
		static SoundManager instance;
		return instance;
	}

	void Initialize();

	void Finalize();

	//ƒTƒEƒ“ƒhŽæ“¾
	AudioSource* GetSound(SoundList sound)const { return sounds[(int)sound]; }

private:
	AudioSource* sounds[SoundList::SoundCount]{};
};
