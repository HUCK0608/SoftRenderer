
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

void WindowsRSI::DrawLine(const ScreenPoint& InPoint1, const ScreenPoint& InPoint2, const LinearColor& InColor)
{
	ScreenPoint startPoint = InPoint1.X < InPoint2.X ? InPoint1 : InPoint2;
	ScreenPoint finalPoint = InPoint1.X < InPoint2.X ? InPoint2 : InPoint1;

	// 제일 왼쪽 점을 구함
	int x = startPoint.X;
	int y = startPoint.Y;

	int W = finalPoint.X - startPoint.X;
	int H = finalPoint.Y - startPoint.Y;

	// 1, 3 사분면 그리기
	if (H > 0)
	{
		H = Math::Abs(H);

		// 기울기가 수평에 가까울 경우
		if (W > H)
		{
			int F = 2 * H - W;

			int dF1 = 2 * H;
			int dF2 = 2 * (H - W);

			for (; x <= finalPoint.X; x++)
			{
				PutPixel(ScreenPoint(x, y), InColor);

				if (F < 0)
				{
					F += dF1;
				}
				else
				{
					++y;
					F += dF2;
				}
			}
		}
		// 기울기가 수직에 가까울 경우
		else
		{
			int F = H - 2 * W;

			int dF1 = -2 * W;
			int dF2 = 2 * (H - W);

			for (; y <= finalPoint.Y; y++)
			{
				PutPixel(ScreenPoint(x, y), InColor);

				if (F >= 0)
				{
					F += dF1;
				}
				else
				{
					++x;
					F += dF2;
				}
			}
		}
	}
	// 2, 4 사분면 그리기
	else
	{
		H = Math::Abs(H);

		// 기울기가 수평에 가까울 경우
		if (W > H)
		{
			int F = 2 * H - W;

			int dF1 = 2 * H;
			int dF2 = 2 * (H - W);

			for (; x <= finalPoint.X; x++)
			{
				PutPixel(ScreenPoint(x, y), InColor);

				if (F < 0)
				{
					F += dF1;
				}
				else
				{
					--y;
					F += dF2;
				}
			}
		}
		// 기울기가 수직에 가까울 경우
		else
		{
			int F = H - 2 * W;

			int dF1 = -2 * W;
			int dF2 = 2 * (H - W);

			for (; y > finalPoint.Y; y--)
			{
				PutPixel(ScreenPoint(x, y), InColor);

				if (F >= 0)
				{
					F += dF1;
				}
				else
				{
					++x;
					F += dF2;
				}
			}
		}
	}
}

void WindowsRSI::DrawLine(const Vector2 & InPoint1, const Vector2 & InPoint2, const LinearColor & InColor)
{
	DrawLine(ScreenPoint(InPoint1), ScreenPoint(InPoint2), InColor);
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
