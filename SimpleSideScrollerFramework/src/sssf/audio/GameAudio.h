#pragma once

#include "sssf_VS\stdafx.h"
#include "sssf\audio\sounds.h"

class GameAudio
{
public:
	GameAudio() {}
	~GameAudio();

	void initAudio();
	void update();
	void suspend();
	void resume();
	void setVolume(float);
	void playSoundFX(const int);
	void playBackgroundSound(const int);

private:
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unique_ptr<DirectX::WaveBank> m_sounds;
	std::unique_ptr<DirectX::SoundEffectInstance> m_background_music;
	bool m_retryAudio;
	float m_volume;
};