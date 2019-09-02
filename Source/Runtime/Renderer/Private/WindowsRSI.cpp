
#include "Precompiled.h"
#include "WindowsRSI.h"

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

void WindowsRSI::DrawScreenPoint(const ScreenPoint & InScreenPos, const LinearColor & InColor)
{
	PutPixel(InScreenPos, InColor.ToColor32());
}
