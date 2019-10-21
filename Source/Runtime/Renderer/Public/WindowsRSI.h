#pragma once

#include <windows.h>
#include "WindowsGDI.h"
#include "RenderingSoftwareInterface.h"

class WindowsRSI : public WindowsGDI, public RenderingSoftwareInterface
{
public:
	WindowsRSI() { };
	~WindowsRSI() { Shutdown(); }

public:
	virtual void Init(const bool InbSRGBColorSpace = true) override;
	virtual void Shutdown() override;

	virtual bool IsIntialized() const override { return bGDIInitialized; }
	virtual bool IsSRGBSpace() const override { return bSRGBColorSpace; }

	virtual void Clear(const LinearColor& InClearColor) override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	virtual void DrawScreenPoint(const ScreenPoint& InPoint, const LinearColor& InColor) override;
	virtual void SetVertexBuffer(Vertex* InVertexData) override;
	virtual void SetIndexBuffer(const int* InIndexData) override;
	virtual int SetTexture(RSITexture& InRSITexture) override;
	virtual void DrawPrimitive(UINT InVertexSize, UINT InIndexSize) override;
	virtual void DrawTopFlatTriangle(Vertex * tvs, bool DrawLastLine) override;
	virtual void DrawBottomFlatTriangle(Vertex * tvs) override;
	virtual void DrawLine(const ScreenPoint& InPoint1, const ScreenPoint& InPoint2, const LinearColor& InColor) override;
	virtual void DrawLine(const Vector2& InPoint1, const Vector2& InPoint2, const LinearColor& InColor) override;
	virtual void DrawHorizontalLine(int InY, const LinearColor& InColor) override;
	virtual void DrawVerticalLine(int inX, const LinearColor& InColor) override;

	LinearColor GetTextureSample(const Vector2& InUV);

private:
	Vertex* VertexBuffer;
	const int* IndexBuffer;
	RSITexture MainTexture;
	bool HasTexture = false;
};