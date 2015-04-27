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
	void playSoundFX(const int);

private:
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unique_ptr<DirectX::WaveBank> m_sounds;
};