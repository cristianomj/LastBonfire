#include "sssf\audio\GameAudio.h"

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

void GameAudio::initAudio()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine.reset(new AudioEngine(eflags));

	m_explode.reset(new SoundEffect(m_audEngine.get(), L"data/audio/explo1.wav"));
	m_ambient.reset(new SoundEffect(m_audEngine.get(), L"data/audio/NightAmbienceSimple_02.wav"));
}

void GameAudio::update()
{
	if (!m_audEngine->Update())
	{
		// more about this below...
	}

	m_ambient->Play();
	m_explode->Play();
}

void GameAudio::suspend()
{
	m_audEngine->Suspend();
}

void GameAudio::resume()
{
	m_audEngine->Resume();
}

