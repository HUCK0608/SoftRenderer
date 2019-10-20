
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"
#include "MathHeaders.h"

void SoftRenderer::Initialize()
{
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
		return;

	MilliSecFrequency = double(frequency.QuadPart) / 1000.0;

	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);
	}
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

static float currentDegree = 0.f;

void SoftRenderer::Update()
{
	if (RSI != nullptr)
	{
		RSI->Clear(LinearColor(0.125f, 0.5f, 1.f, 1.f)); 

		//RenderCode

		// Axis Line
		RSI->DrawHorizontalLine(0, LinearColor(1.f, 0.f, 0.f, 1.f));
		RSI->DrawVerticalLine(0, LinearColor(0.f, 1.f, 0.f, 1.f));

		// Grid Line
		ScreenPoint screenSize = DisplaySetting::Inst().GetSize();

		
		int gridSize = 10;
		int halfX = Math::FloorToInt(((float)screenSize.X - 1.f) * 0.5f);
		int halfY = Math::FloorToInt(((float)screenSize.Y - 1.f) * 0.5f);

		for (int x = gridSize; x < halfX; x += gridSize)
		{
			RSI->DrawVerticalLine(x, LinearColor(0.5f, 0.5f, 0.5f, 1.f));
			RSI->DrawVerticalLine(-x, LinearColor(0.5f, 0.5f, 0.5f, 1.f));
		}

		for (int y = gridSize; y < halfY; y += gridSize)
		{
			RSI->DrawHorizontalLine(y, LinearColor(0.5f, 0.5f, 0.5f, 1.f));
			RSI->DrawHorizontalLine(-y, LinearColor(0.5f, 0.5f, 0.5f, 1.f));
		}

		/*RSI->DrawLine(Vector2(30.0f, 30.0f), Vector2(60.0f, 200.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(30.0f, 30.0f), Vector2(200.0f, 60.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(-30.0f, 30.0f), Vector2(-60.0f, 200.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(-30.0f, 30.0f), Vector2(-200.0f, 60.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(-30.0f, -30.0f), Vector2(-60.0f, -200.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(-30.0f, -30.0f), Vector2(-200.0f, -60.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(30.0f, -30.0f), Vector2(60.0f, -200.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(30.0f, -30.0f), Vector2(200.0f, -60.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));*/

		Matrix3x3 tMat, rMat, sMat;

		tMat.SetTranslation(Vector2(100.f, 200.f));

		float degreeTo1Sec = 180.f;
		currentDegree = GetFrameFPS() == 0 ? 0 : currentDegree + degreeTo1Sec / GetFrameFPS();
		rMat.SetRotation(currentDegree);

		float sinWave = sinf((ElapsedTime * Math::TwoPI) + 1) * 0.5f;
		float minScale = 0.8f;
		float maxScale = 1.6f;
		float currentScale = minScale + sinWave * (maxScale - minScale);
		sMat.SetScale(Vector2(currentScale, currentScale));

		Matrix3x3 TRSMat = tMat * rMat * sMat;

		// Set Vertex
		VertexData v[4];
		v[0].Position = Vector2(100.0f, 100.0f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].Position = Vector2(-100.0f, -100.0f);
		v[1].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[2].Position = Vector2(-100.0f, 100.0f);
		v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);
		v[3].Position = Vector2(100.0f, -100.0f);
		v[3].Color = LinearColor(0.0f, 1.0f, 0.0f);

		for (int i = 0; i < 4; i++)
		{
			v[i].Position = TRSMat * v[i].Position;
		}

		// Set Index
		int i[6];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 0;
		i[4] = 1;
		i[5] = 3;

		// Draw Primitive
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);
		RSI->DrawPrimitive();

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


