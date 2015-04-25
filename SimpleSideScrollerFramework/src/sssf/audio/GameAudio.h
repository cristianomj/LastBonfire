#pragma once

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
#include "Audio.h"

class GameAudio
{
public:
	GameAudio() {}
	~GameAudio();

	void initAudioEngine();
	void update();
	void suspend();
	void resume();

private:
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
};