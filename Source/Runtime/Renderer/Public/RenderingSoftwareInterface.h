#pragma once

#include "DisplaySetting.h"
#include "LinearColor.h"
#include "Vertex.h"

struct RSITexture
{
	RSITexture() = default;
	RSITexture(LinearColor* InTextureBuffer, UINT InWidth, UINT InHeight) : TextureBuffer(InTextureBuffer), Width(InWidth), Height(InHeight) {}
	UINT Width = 0;
	UINT Height = 0;
	LinearColor* TextureBuffer = nullptr;
};

class RenderingSoftwareInterface
{
public:
	virtual void Init(const bool InbSRGBColorSpace = true) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsIntialized() const = 0;
	virtual bool IsSRGBSpace() const = 0;

	virtual void Clear(const LinearColor& InClearColor) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void DrawScreenPoint(const ScreenPoint& InPoint,const LinearColor& InColor) = 0;

	virtual void SetVertexBuffer(Vertex* InVertexData) = 0;
	virtual void SetIndexBuffer(const int* InIndexData) = 0;
	virtual int SetTexture(RSITexture& InRSITexture) = 0;
	virtual void DrawPrimitive(UINT InVertexSize, UINT InIndexSize) = 0;
	virtual void DrawBottomFlatTriangle(Vertex * tvs) = 0;
	virtual void DrawTopFlatTriangle(Vertex * tvs, bool DrawLastLine) = 0;
	virtual void DrawLine(const ScreenPoint& InPoint1, const ScreenPoint& InPoint2, const LinearColor& InColor) = 0;
	virtual void DrawLine(const Vector2& InPoint1, const Vector2& InPoint2, const LinearColor& InColor) = 0;
	virtual void DrawHorizontalLine(int InY, const LinearColor& InColor) = 0;
	virtual void DrawVerticalLine(int InX, const LinearColor& InColor) = 0;
};