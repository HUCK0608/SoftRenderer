
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"
#include "MathHeaders.h"

void SoftRenderer::Initialize()
{
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

		RSI->DrawLine(Vector2(30.0f, 30.0f), Vector2(60.0f, 200.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(30.0f, 30.0f), Vector2(200.0f, 60.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(-30.0f, 30.0f), Vector2(-60.0f, 200.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(-30.0f, 30.0f), Vector2(-200.0f, 60.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(-30.0f, -30.0f), Vector2(-60.0f, -200.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(-30.0f, -30.0f), Vector2(-200.0f, -60.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(30.0f, -30.0f), Vector2(60.0f, -200.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));
		RSI->DrawLine(Vector2(30.0f, -30.0f), Vector2(200.0f, -60.0f), LinearColor(0.0f, 0.0f, 1.0f, 1.0f));

		//// Set Vertex
		//VertexData v[4];
		//v[0].Position = Vector2(100.0f, 100.0f);
		//v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		//v[1].Position = Vector2(-100.0f, -100.0f);
		//v[1].Color = LinearColor(1.0f, 0.0f, 0.0f);
		//v[2].Position = Vector2(-100.0f, 100.0f);
		//v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);
		//v[3].Position = Vector2(100.0f, -100.0f);
		//v[3].Color = LinearColor(0.0f, 1.0f, 0.0f);

		//// Set Index
		//int i[6];
		//i[0] = 0;
		//i[1] = 1;
		//i[2] = 2;
		//i[3] = 0;
		//i[4] = 1;
		//i[5] = 3;

		//// Draw Primitive
		//RSI->SetVertexBuffer(v);
		//RSI->SetIndexBuffer(i);
		//RSI->DrawPrimitive();

		RSI->EndFrame();
	}
}


