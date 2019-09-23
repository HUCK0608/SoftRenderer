
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "TriangleRasterizer.h"

void WindowsRSI::Init(const bool InbSRGBColorSpace)
{
	InitializeGDI(DisplaySetting::Inst().GetSize(), InbSRGBColorSpace);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	SetColor(InClearColor);
	FillBuffer();

	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{

}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawScreenPoint(const ScreenPoint & InPoint, const LinearColor & InColor)
{
	PutPixel(InPoint,InColor.ToColor32());
}

void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int * InIndexData)
{
	IndexBuffer = InIndexData;
}

void WindowsRSI::DrawPrimitive()
{

	if (VertexBuffer == NULL || IndexBuffer == NULL) return;

	UINT triangleCount = (int)(sizeof(IndexBuffer) / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		TriangleRasterizer t(
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]
		);

		for (int x = t.TopLeft.X; x < t.BottomRight.X; x++)
		{
			for (int y = t.TopLeft.Y; y < t.BottomRight.Y; y++)
			{
				ScreenPoint curPixel(x, y);
				Vector2 curPos = curPixel.ToVector2();

				if (t.IsInside(curPos))
				{
					PutPixel(curPixel, t.GetColor(curPos));
				}
			}
		}
	}
}

void WindowsRSI::DrawHorizontalLine(int InY, const LinearColor & InColor)
{
	Color32 color = InColor.ToColor32();
	Color32* dest = ScreenBuffer;

	int xStartIndex = (Math::FloorToInt(((float)ScreenSize.Y - 1.f) * 0.5f) - InY) * ScreenSize.X;
	for (int x = 0; x < ScreenSize.X; ++x)
	{
		*(dest + xStartIndex) = color;
		dest++;
	}
}

void WindowsRSI::DrawVerticalLine(int inX, const LinearColor & InColor)
{
	Color32 color = InColor.ToColor32();
	Color32* dest = ScreenBuffer;

	int xIndex = inX + Math::FloorToInt(((float)ScreenSize.X - 1.f) * 0.5f);
	for (int y = 0; y < ScreenSize.Y; ++y)
	{
		*(dest + xIndex) = color;
		dest += ScreenSize.X;
	}
}
