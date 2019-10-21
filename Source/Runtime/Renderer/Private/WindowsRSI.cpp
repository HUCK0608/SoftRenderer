
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

void WindowsRSI::SetVertexBuffer(Vertex * InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int * InIndexData)
{
	IndexBuffer = InIndexData;
}

int WindowsRSI::SetTexture(RSITexture & InRSITexture)
{
	MainTexture.TextureBuffer = InRSITexture.TextureBuffer;
	MainTexture.Width = InRSITexture.Width;
	MainTexture.Height = InRSITexture.Height;
	HasTexture = true;
	return 0;
}

void WindowsRSI::DrawTopFlatTriangle(Vertex * tvs, bool DrawLastLine = false)
{
	float dx1 = tvs[0].Position.X - tvs[2].Position.X;
	float dx2 = tvs[1].Position.X - tvs[2].Position.X;
	float dy = tvs[2].Position.Y - tvs[1].Position.Y;

	if (dy >= 0)
	{
		return;
	}

	float gradient1 = dx1 / dy;
	float gradient2 = dx2 / dy;

	PutPixel(ScreenPoint(tvs[2].Position), LinearColor(1.f, 0.f, 0.f));
	float startY = tvs[2].Position.Y;
	float startX = tvs[2].Position.X;
	float currentY = floorf(tvs[2].Position.Y) - 0.5f;
	float destY = tvs[1].Position.Y;
	while (currentY <= destY)
	{
		float deltaY = startY - currentY;
		float leftX = gradient1 * deltaY + startX;
		float rightX = gradient2 * deltaY + startX;
		int pixelX1 = Math::FloorToInt(leftX);
		int pixelX2 = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(currentY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
		currentY += 1.0f;
	}

	if (DrawLastLine)
	{
		// 마지막 라인을 그린다.
		int pixelX1 = Math::FloorToInt(tvs[1].Position.X);
		int pixelX2 = Math::FloorToInt(tvs[2].Position.X);
		int pixelY = Math::FloorToInt(destY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
	}
}

void WindowsRSI::DrawBottomFlatTriangle(Vertex * tvs)
{
	float dx1 = tvs[1].Position.X - tvs[0].Position.X;
	float dx2 = tvs[2].Position.X - tvs[0].Position.X;
	float dy = tvs[0].Position.Y - tvs[1].Position.Y;

	if (dy <= 0)
	{
		return;
	}

	float gradient1 = dx1 / dy;
	float gradient2 = dx2 / dy;

	PutPixel(ScreenPoint(tvs[0].Position), LinearColor(0.f, 1.f, 0.f));
	float startY = tvs[0].Position.Y;
	float startX = tvs[0].Position.X;
	float currentY = floorf(tvs[0].Position.Y) - 0.5f;
	float destY = tvs[1].Position.Y;
	while (currentY >= destY)
	{
		float deltaY = startY - currentY;
		float leftX = gradient1 * deltaY + startX;
		float rightX = gradient2 * deltaY + startX;
		int startX = Math::FloorToInt(leftX);
		int endX = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(currentY);
		for (int p = startX; p <= endX; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
		}
		currentY -= 1.0f;
	}

	// 마지막 라인을 그린다.
	int pixelX1 = Math::FloorToInt(tvs[0].Position.X);
	int pixelX2 = Math::FloorToInt(tvs[1].Position.X);
	int pixelY = Math::FloorToInt(destY);
	for (int p = pixelX1; p <= pixelX2; ++p)
	{
		PutPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
	}
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{
	if (VertexBuffer == nullptr || IndexBuffer == nullptr)
	{
		return;
	}

	UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		// Draw Each Triangle List
		TriangleRasterizer t(
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]);
		t.RecalcBounds();
		for (int x = t.TopLeft.X; x < t.BottomRight.X; ++x)
		{
			for (int y = t.TopLeft.Y; y < t.BottomRight.Y; ++y)
			{
				ScreenPoint currentPixel(x, y);
				Vector2 currentPos = currentPixel.ToVector2();
				if (t.IsInside(currentPos))
				{
					if (HasTexture)
					{
						PutPixel(currentPixel, GetTextureSample(t.GetUV(currentPos)));
					}
					else
					{
						PutPixel(currentPixel, t.GetColor(currentPos));
					}
				}
			}
		}
	}

	/*UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		Vertex tv[3] = {
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]
		};
		Vertex temp;

		if (tv[0].Position.Y == tv[1].Position.Y)
		{
			if (tv[0].Position.X > tv[1].Position.X)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}
		else
		{
			if (tv[0].Position.Y < tv[1].Position.Y)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}

		if (tv[1].Position.Y == tv[2].Position.Y)
		{
			if (tv[1].Position.X > tv[2].Position.X)
			{
				temp = tv[1];
				tv[1] = tv[2];
				tv[2] = temp;
			}
		}
		else
		{
			if (tv[1].Position.Y < tv[2].Position.Y)
			{
				temp = tv[1];
				tv[1] = tv[2];
				tv[2] = temp;
			}
		}


		if (tv[0].Position.Y == tv[1].Position.Y)
		{
			if (tv[0].Position.X > tv[1].Position.X)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}
		else
		{
			if (tv[0].Position.Y < tv[1].Position.Y)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}

		if (tv[0].Position.Y == tv[1].Position.Y)
		{
			DrawTopFlatTriangle(tv);
		}
		else if (tv[1].Position.Y == tv[2].Position.Y)
		{
			DrawBottomFlatTriangle(tv);
		}
		else
		{
			Vertex newV = tv[1];
			float height = tv[0].Position.Y - tv[2].Position.Y;
			float width = tv[2].Position.X - tv[0].Position.X;

			if (height == 0.0f)
			{
				return;
			}

			float gradient = width / height;
			newV.Position.X = gradient * (tv[0].Position.Y - tv[1].Position.Y) + tv[0].Position.X;

			if (newV.Position.X > tv[1].Position.X)
			{
				Vertex upperTriangle[3] = { tv[0], tv[1], newV };
				Vertex bottomTriangle[3] = { tv[1], newV, tv[2] };
				DrawTopFlatTriangle(bottomTriangle);
				DrawBottomFlatTriangle(upperTriangle);
			}
			else
			{
				Vertex upperTriangle[3] = { tv[0], newV, tv[1] };
				Vertex bottomTriangle[3] = { newV, tv[1], tv[2] };
				DrawTopFlatTriangle(bottomTriangle);
				DrawBottomFlatTriangle(upperTriangle);
			}
		}
	}*/
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

LinearColor WindowsRSI::GetTextureSample(const Vector2 & InUV)
{
	UINT width = MainTexture.Width;
	UINT height = MainTexture.Height;
	UINT pixelX = Math::FloorToInt(InUV.X * width);
	UINT pixelY = Math::FloorToInt(InUV.Y * height);
	pixelX %= width;
	pixelY %= height;
	UINT textureIndex = width * pixelY + pixelX;
	return MainTexture.TextureBuffer[textureIndex];
}
