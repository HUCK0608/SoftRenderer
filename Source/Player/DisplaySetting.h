#pragma once
#include "ScreenPoint.h"

class DisplaySetting
{
private:
	ScreenPoint _size;

public:
	DisplaySetting(DisplaySetting const&) = delete;
	void operator=(DisplaySetting const&) = delete;

	static DisplaySetting& Inst()
	{
		static DisplaySetting instance;
		return instance;
	}

	void SetSize(ScreenPoint size) { _size = size; }
	ScreenPoint GetSize() { return _size; }

private:
	DisplaySetting() {}
	~DisplaySetting() {}
};