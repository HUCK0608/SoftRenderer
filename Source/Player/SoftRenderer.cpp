
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


