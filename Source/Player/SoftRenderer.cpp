
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"
#include "MathHeaders.h"
#include "EngineHeaders.h"

void SoftRenderer::Initialize()
{
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
		return;

	MilliSecFrequency = double(frequency.QuadPart) / 1000.0;

	RSI = new WindowsRSI();
	if (RSI == nullptr)
		return;

	RSI->Init(false);
	MainTexture.LoadPNGFile("Steve.png");
}

void SoftRenderer::Shutdown()
{
	if (RSI != nullptr && RSI->IsIntialized())
	{
		RSI->Shutdown();
		delete RSI;
		RSI = nullptr;
	}
}

void SoftRenderer::PreUpdate()
{
	StartFrameSec = CheckMilliSeconds();
}

static Vector2 currentPosition;
static float currentDegree = 0.f;

void SoftRenderer::Update()
{
	if (RSI != nullptr)
	{
		RSI->Clear(LinearColor(0.125f, 0.5f, 1.f, 1.f)); 

		// Set Vertex
		Vertex v[4];
		v[0].Position = Vector2(100.0f, 100.0f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[0].UV = Vector2(0.125f, 0.125f);
		v[1].Position = Vector2(-100.0f, -100.0f);
		v[1].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].UV = Vector2(0.25f, 0.25f);
		v[2].Position = Vector2(-100.0f, 100.0f);
		v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);
		v[2].UV = Vector2(0.25f, 0.125f);
		v[3].Position = Vector2(100.0f, -100.0f);
		v[3].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[3].UV = Vector2(0.125f, 0.25f);

		// Set Index
		int i[6];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 0;
		i[4] = 1;
		i[5] = 3;

		// Draw Primitive
		RSI->SetTexture(RSITexture(MainTexture.GetBuffer(), MainTexture.GetWidth(), MainTexture.GetHeight()));
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);

		// Render & Transform
		static GameObject2D quad;

		float degreeTo1Sec = 180.f;
		currentDegree = GetFrameFPS() == 0 ? 0 : currentDegree + degreeTo1Sec / GetFrameFPS();

		float sinWave = sinf((ElapsedTime * Math::TwoPI) + 1) * 0.5f;
		float minScale = 0.8f;
		float maxScale = 1.6f;
		float currentScale = minScale + sinWave * (maxScale - minScale);

		quad.GetTransform().SetPosition(Vector2(0.f, 0.f));
		quad.GetTransform().SetRotation(currentDegree);
		quad.GetTransform().SetScale(Vector2(currentScale, currentScale));
		Matrix3x3 TRSMat = quad.GetTransform().GetTRS();

		// Camera Movement
		static Camera2D camera2D;

		float moveSpeed = 300.f;
		currentPosition.X += GetFrameFPS() == 0.f ? 0.f : (moveSpeed / GetFrameFPS()) * InputManager.GetXAxis();
		currentPosition.Y += GetFrameFPS() == 0.f ? 0.f : (moveSpeed / GetFrameFPS()) * InputManager.GetYAxis();

		camera2D.GetGameObject().GetTransform().SetPosition(currentPosition);

		Matrix3x3 finalMatrix = camera2D.GetViewMatrix();

		for (int i = 0; i < 4; i++)
		{
			v[i].Position = finalMatrix * v[i].Position;
		}

		RSI->DrawPrimitive(4, 6);

		RSI->EndFrame();
	}
}

void SoftRenderer::PostUpdate()
{
	FrameCount++;

	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	LONGLONG elapsedCycles = currentTime.QuadPart - StartFrameSec;

	FramePerMilliSeconds = double(elapsedCycles / 1000.0) / (MilliSecFrequency / 1000.0);
	FramePerSeconds = (float)(FramePerMilliSeconds / 1000.0);
	ElapsedTime += FramePerSeconds;
	FrameFPS = FramePerSeconds == 0 ? 0 : 1 / FramePerSeconds;
}

LONGLONG SoftRenderer::CheckMilliSeconds()
{
	LARGE_INTEGER cycles;
	QueryPerformanceCounter(&cycles);

	return cycles.QuadPart;
}


