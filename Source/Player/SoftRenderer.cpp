
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"

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
		// RSI->BeginFrame();
		RSI->Clear(LinearColor(0.125f, 0.5f, 1.f, 1.f));
		
		// Render Code
		RSI->DrawScreenPoint(ScreenPoint(0, 0), LinearColor(1.f, 0.f, 0.f, 1.f));

		// Draw Circle
		Vector2 center(0.f, 0.f);
		float radius = 30.f;

		Vector2 minPos = Vector2(center.X - radius, center.Y - radius);
		Vector2 maxPos = Vector2(center.X + radius, center.Y + radius);

		ScreenPoint minPosScreen(minPos);
		ScreenPoint maxPosScreen(maxPos);

		LinearColor circleColor(1.f, 0.f, 0.f, 1.f);
		for (int x = minPosScreen.X; x <= maxPosScreen.X; x++)
		{
			for (int y = minPosScreen.Y; y <= maxPosScreen.Y; y++)
			{
				ScreenPoint targetPixel(x, y);
				Vector2 targetPos = targetPixel.ToVector2();
				float distSquare = (targetPos - center).SizeSquared();

				if(distSquare <= radius * radius)
					RSI->DrawScreenPoint(targetPixel, circleColor);
			}
		}

		RSI->EndFrame();
	}
}


