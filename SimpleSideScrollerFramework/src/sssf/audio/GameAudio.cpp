#include "GameAudio.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

GameAudio::~GameAudio()
{
	if (m_audEngine)
	{
		m_audEngine->Suspend();
	}
}

void GameAudio::initAudioEngine()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine.reset(new AudioEngine(eflags));
}

void GameAudio::update()
{
	if (!m_audEngine->Update())
	{

	}
}

void GameAudio::suspend()
{
	m_audEngine->Suspend();
}

void GameAudio::resume()
{
	m_audEngine->Resume();
}