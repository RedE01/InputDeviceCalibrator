#pragma once
#include "../../ScreenInfo.h"

class X11ScreenInfo : public ScreenInfo {
public:
	X11ScreenInfo(const char* screenName);
};