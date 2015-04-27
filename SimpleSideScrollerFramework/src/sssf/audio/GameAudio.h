#pragma once

#include "sssf_VS\stdafx.h"

class GameAudio
{
public:
	GameAudio() {}
	~GameAudio();

	void initAudio();
	void update();
	void suspend();
	void resume();

private:
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	std::unique_ptr<DirectX::SoundEffect> m_explode;
	std::unique_ptr<DirectX::SoundEffect> m_ambient;
};