#include "sssf\audio\GameAudio.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

GameAudio::GameAudio()
{
	m_musicEnabled = true;
	m_soundEffectEnabled = true;
	m_volume = 1.0f;
	m_retryAudio = false;
}
GameAudio::~GameAudio()
{
	if (m_audioEngine)
	{
		m_audioEngine->Suspend();
	}
}

// CALLED TO INITIALIZE AUDIO ENGINE
void GameAudio::initAudio()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audioEngine.reset(new AudioEngine(eflags));

	m_sounds.reset(new WaveBank(m_audioEngine.get(), L"data/audio/sounds.xwb"));

	playBackgroundSound(XACT_WAVEBANK_SOUNDS_BACKGROUND);
}

// CALLED EACH FRAME TO UPDATE AUDIO ENGINE
// IF AUDIO DEVICE IS LOST, LOOK FOR NEW ONE
void GameAudio::update()
{
	if (m_retryAudio)
	{
		m_retryAudio = false;
		if (m_audioEngine->Reset())
		{
			// TODO: restart any looped sounds here
			if (m_background_music)
				m_background_music->Play(true);
		}
	}

	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			m_retryAudio = true;
		}
	}
}

// PLAYS A ONE TIME SOUND EFFECT
void GameAudio::playSoundFX(const int sfx)
{
	if (m_soundEffectEnabled) {
		m_sounds->Play(sfx);
	}	
}

// PLAYS A LOOPING SOUND - USE FOR BACKGROUND MUSIC
void GameAudio::playBackgroundSound(const int sfx)
{
	if (m_musicEnabled) {
		m_background_music = m_sounds->CreateInstance(sfx);
		m_background_music->Play(true);
	}
}