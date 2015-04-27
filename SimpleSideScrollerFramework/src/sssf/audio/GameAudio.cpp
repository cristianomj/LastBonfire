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

	m_sounds.reset(new WaveBank(m_audioEngine.get(), L"data/audio/sounds.xwb"));
}

void GameAudio::update()
{
	if (!m_audioEngine->Update())
	{
		// more about this below...
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

