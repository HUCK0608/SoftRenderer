#pragma once

#include "WindowsRSI.h"
#include "InputManager.h"
#include "Texture.h"

class SoftRenderer
{
public:
	enum class RenderMode
	{
		TWO,
		THREE_PERSP
	};

	SoftRenderer(SoftRenderer const&) = delete;
	void operator=(SoftRenderer const&) = delete;
	static SoftRenderer& Inst()
	{
		static SoftRenderer instance;
		return instance;
	}

public:
	void SetRenderMode(RenderMode InRenderMode) { CurrentRenderMode = InRenderMode; }
	RenderMode GetRenderMode() const { return CurrentRenderMode; }
	InputManager& GetInputManager() { return InputManager; }
	void Initialize();
	void Shutdown();
	void PreUpdate();
	void Update();
	void PostUpdate();

	LONGLONG CheckMilliSeconds();
	float GetFrameFPS() const {return FrameFPS;}
	float GetAverageFPS() const {return FrameCount == 0.f ? 0.f : FrameCount / ElapsedTime;}
	float GetElapsedTime() const {return ElapsedTime;}
	int GetFrameCount() const {return FrameCount;}

private:
	SoftRenderer() { }
	~SoftRenderer() { Shutdown(); }

	RenderMode CurrentRenderMode = RenderMode::TWO;

	WindowsRSI* RSI = nullptr;

	double MilliSecFrequency = 0;
	double FramePerMilliSeconds = 0;
	float FramePerSeconds = 0;
	float FrameFPS = 0;
	float ElapsedTime = 0;
	LONGLONG StartFrameSec = 0;
	int FrameCount = 0;

	InputManager InputManager;
	Texture MainTexture;
};
