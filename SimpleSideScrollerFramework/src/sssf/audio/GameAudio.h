#pragma once
/*
	Author: Cristiano Miranda
*/

#include "sssf_VS\stdafx.h"
#include "sssf\audio\sounds.h"

class GameAudio
{
public:
	GameAudio();
	~GameAudio();

	// INLINE FUNCTIONS
	void suspend()					{ m_audioEngine->Suspend();						}
	void resume()					{ m_audioEngine->Resume();						}
	void setVolume(float newVolume) { m_background_music->SetVolume(newVolume);		}
	void toggleSoundEffectEnabled() { m_soundEffectEnabled = !m_soundEffectEnabled; }
	void toggleMusicEnabled()		{ m_musicEnabled = !m_musicEnabled;				}

	// FUNCTIONS DEFINED IN GameAudio.cpp
	void initAudio();
	void update();
	void playSoundFX(const int);
	void playBackgroundSound(const int);

private:
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unique_ptr<DirectX::WaveBank> m_sounds;
	std::unique_ptr<DirectX::SoundEffectInstance> m_background_music;
	bool m_retryAudio;
	bool m_soundEffectEnabled;
	bool m_musicEnabled;
	float m_volume;

};