#include "System/Audio.h"
#include "SoundManager.h"

void SoundManager::Initialize()
{
	//sounds[SoundList::titleBGM] = Audio::Instance().LoadAudioSource("Data/Sound/TitleBGM.wav");
	sounds[SoundList::titleBGM] = Audio::Instance().LoadAudioSource("Data/Sound/TitleBGM.wav");
	sounds[SoundList::gameBGM] = Audio::Instance().LoadAudioSource("Data/Sound/BGM.wav");
	sounds[SoundList::resultOverBGM] = Audio::Instance().LoadAudioSource("Data/Sound/ResultBGM.wav");
	/*sounds[SoundList::resultClearBGM] = Audio::Instance().LoadAudioSource("Data/Sound/gameClear.wav");
	sounds[SoundList::walkSE] = Audio::Instance().LoadAudioSource("Data/Sound/walk.wav");
	sounds[SoundList::numSE] = Audio::Instance().LoadAudioSource("Data/Sound/selectNum.wav");
	sounds[SoundList::flagSE] = Audio::Instance().LoadAudioSource("Data/Sound/flag.wav");
	sounds[SoundList::startSE] = Audio::Instance().LoadAudioSource("Data/Sound/start.wav");
	sounds[SoundList::heartSE] = Audio::Instance().LoadAudioSource("Data/Sound/heart.wav");*/
	sounds[SoundList::SlideSE] = Audio::Instance().LoadAudioSource("Data/Sound/slide.wav");
	sounds[SoundList::playerSE] = Audio::Instance().LoadAudioSource("Data/Sound/player.wav");
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