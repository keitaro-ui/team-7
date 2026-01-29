#include "System/Audio.h"
#include "SoundManager.h"

void SoundManager::Initialize()
{
	sounds[SoundList::titleBGM] = Audio::Instance().LoadAudioSource("Data/Sound/TitleBGM.wav");
	sounds[SoundList::titleBGM] = Audio::Instance().LoadAudioSource("Data/Sound/TitleBGM.wav");
	sounds[SoundList::gameBGM] = Audio::Instance().LoadAudioSource("Data/Sound/GameBGM.wav");
	sounds[SoundList::resultOverBGM] = Audio::Instance().LoadAudioSource("Data/Sound/gameOver.wav");
	sounds[SoundList::resultClearBGM] = Audio::Instance().LoadAudioSource("Data/Sound/gameClear.wav");
	sounds[SoundList::walkSE] = Audio::Instance().LoadAudioSource("Data/Sound/walk.wav");
	sounds[SoundList::numSE] = Audio::Instance().LoadAudioSource("Data/Sound/selectNum.wav");
	sounds[SoundList::flagSE] = Audio::Instance().LoadAudioSource("Data/Sound/flag.wav");
	sounds[SoundList::startSE] = Audio::Instance().LoadAudioSource("Data/Sound/start.wav");
	sounds[SoundList::heartSE] = Audio::Instance().LoadAudioSource("Data/Sound/heart.wav");
}

void SoundManager::Finalize()
{
	for (int i = 0; i < SoundList::SoundCount; i++)
	{
		if (sounds[i] != nullptr)
		{
			delete sounds[i];
			sounds[i] = nullptr;
		}
	}
}