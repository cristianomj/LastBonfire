#include "sssf\audio\GameAudio.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

GameAudio::~GameAudio()
{
	if (m_audioEngine)
	{
		m_audioEngine->Suspend();
	}
}

void GameAudio::initAudio()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audioEngine.reset(new AudioEngine(eflags));

	m_retryAudio = false;
	m_volume = 1.0f;

	m_sounds.reset(new WaveBank(m_audioEngine.get(), L"data/audio/sounds.xwb"));

	playBackgroundSound(XACT_WAVEBANK_SOUNDS_BACKGROUND);
}

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

void GameAudio::suspend()
{
	m_audioEngine->Suspend();
}

void GameAudio::resume()
{
	m_audioEngine->Resume();
}

void GameAudio::playSoundFX(const int sfx)
{
	m_sounds->Play(sfx);
}

void GameAudio::playBackgroundSound(const int sfx)
{
	m_background_music = m_sounds->CreateInstance(sfx);
	m_background_music->SetVolume(m_volume);
	m_background_music->Play(true);
}

void GameAudio::setVolume(float volume)
{
	m_background_music->SetVolume(volume);
}

