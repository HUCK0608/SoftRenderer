#pragma once
#include "ScreenPoint.h"

/*
DisplaySetting을 WindowsApp.cpp에서 static 변수로 안두고 싱글톤으로 만드는 이유?
 > 초기화 실행 시점. 어디서 초기화 되는가
  static으로 하면 프로그램 실행시 무조건 초기화가 됨  > 호출할지 안할지 모르는데 로딩시 시간이 많이 걸리게 될 것
  싱글톤으로 하면 호출되는 시점에서 초기화가 됨 
  원하는 시점에서 생성을 호출하는 기법 ?lazy intialization (게으른 초기화 기법) 개념을 알아두면 좋다.
*/

class DisplaySetting 
{
	public:
	DisplaySetting(DisplaySetting const&) = delete;
	void operator=(DisplaySetting const&) = delete;
	static DisplaySetting& Inst()
	{
		static DisplaySetting instance;
		return instance;
	}

public:
	void SetSize(ScreenPoint inSize) { size = inSize; }
	ScreenPoint GetSize() { return size; }

private:
	ScreenPoint size;

private:
	DisplaySetting() { }
	~DisplaySetting() { }
};